#include "ControllerDisplay.h"

using namespace Sensors;

#ifdef ESP8266
#define STMPE_CS 16
#define TFT_CS   0
#define TFT_DC   15
#define SD_CS    2
#endif
#ifdef ESP32
#define STMPE_CS 32
#define TFT_CS   15
#define TFT_DC   33
#define SD_CS    14
#endif
#ifdef TEENSYDUINO
#define TFT_DC   10
#define TFT_CS   4
#define STMPE_CS 3
#define SD_CS    8
#endif
#ifdef ARDUINO_STM32_FEATHER
#define TFT_DC   PB4
#define TFT_CS   PA15
#define STMPE_CS PC7
#define SD_CS    PC5
#endif
#ifdef ARDUINO_NRF52_FEATHER /* BSP 0.6.5 and higher! */
#define TFT_DC   11
#define TFT_CS   31
#define STMPE_CS 30
#define SD_CS    27
#endif
#if defined(ARDUINO_MAX32620FTHR) || defined(ARDUINO_MAX32630FTHR)
#define TFT_DC   P5_4
#define TFT_CS   P5_3
#define STMPE_CS P3_3
#define SD_CS    P3_2
#endif

// Anything else!
#if defined (__AVR_ATmega32U4__) || defined(ARDUINO_SAMD_FEATHER_M0) || defined (__AVR_ATmega328P__) || defined(ARDUINO_SAMD_ZERO) || defined(__SAMD51__) || defined(__SAM3X8E__)
#define STMPE_CS 6
#define TFT_CS   9
#define TFT_DC   10
#define SD_CS    5
#endif

namespace Display {
	const char *ControllerDisplay::ExampleOfALabel = "My Label";

	/// <summary>Initializes a new instance of the <see cref="ControllerDisplay"/> class.</summary>
	ControllerDisplay::ControllerDisplay() :
		tft(TFT_CS, TFT_DC),
		humidityArea(0, 0, 150, 106),
		temperatureArea(0, 106, 150, 106)
	{}

	/// <summary>Executes initialization logic for the object.</summary>
	/// <returns>An <see cref="InitializationResult"/> that describes the result of initialization.</returns>
	InitializationResult ControllerDisplay::Initialize()
	{
		InitializationResult result;

		tft.begin();

		noInterrupts();
		tft.setRotation(ControllerDisplay::ScreenOrientation);
		interrupts();

		this->height = tft.height();
		this->width = tft.width();

		result.IsSuccessful = true;
		return result;
	}

	/// <summary>Clears all displayed content from the screen by wiping it with a black background.</summary>
	void ControllerDisplay::Clear()
	{
		noInterrupts();
		tft.fillScreen(ControllerDisplay::BackgroundColor);
		interrupts();
	}

	/// <summary>Draws the initial static elements of the display.</summary>
	void ControllerDisplay::DrawLayout()
	{
		noInterrupts();

		// for centering
		int16_t centeredTextXPosition;

		// for Humidity
		char *humidityLabel = "% Humidity";
		tft.drawRect(humidityArea.x, humidityArea.y, humidityArea.width, humidityArea.height, ControllerDisplay::LayoutLineColor);
		tft.setFont(&FreeSansBold9pt7b);
		tft.setTextSize(1);
		centeredTextXPosition = GetCenteredPosition(humidityLabel, 0, 90, 150);
		tft.setCursor(centeredTextXPosition, 90);
		tft.println(humidityLabel);

		// for temperature
		char *temperatureLabel = "Fahrenheit";
		tft.drawRect(temperatureArea.x, temperatureArea.y, temperatureArea.width, temperatureArea.height, ControllerDisplay::LayoutLineColor);
		tft.setFont(&FreeSansBold9pt7b);
		tft.setTextSize(1);
		centeredTextXPosition = GetCenteredPosition(temperatureLabel, 0, 196, 150);
		tft.setCursor(centeredTextXPosition, 196);
		tft.println(temperatureLabel);

		interrupts();
	}

	/// <summary>Prints the amount of free memory in bytes to the screen.</summary>
	/// <param name="freeMemory">The amount of free memory to display, in bytes.</param>
	void ControllerDisplay::PrintFreeMemory(int freeMemory)
	{
		noInterrupts();

		tft.setFont(&FreeSansBold9pt7b);
		tft.setTextSize(1);

		char *memoryLabel = "Free SRAM: ";

		// overwrite
		tft.setCursor(156, 120);
		tft.setTextColor(ControllerDisplay::BackgroundColor);
		tft.print(memoryLabel);
		tft.print(previousFreeMemory);

		// print the value
		tft.setCursor(156, 120);
		tft.setTextColor(ControllerDisplay::ReadingsTextColor);
		tft.print(memoryLabel);
		tft.print(freeMemory);

		this->previousFreeMemory = freeMemory;

		interrupts();
	}

	/// <summary>Prints an error message to the display.</summary>
	/// <param name="message">The message to display as an error.</param>
	void ControllerDisplay::PrintError(const __FlashStringHelper *message)
	{
		noInterrupts();

		tft.setFont();
		tft.setTextSize(1);

		char *errorLabel = "Error Happened!: ";

		// print the label
		tft.setCursor(156, 136);
		tft.setTextColor(ControllerDisplay::LayoutTextColor);
		tft.print(errorLabel);

		// overwrite
		tft.setCursor(156, 146);
		tft.setTextColor(ControllerDisplay::BackgroundColor);
		tft.print(previousError);

		// print the value
		tft.setCursor(156, 146);
		tft.setTextColor(ControllerDisplay::ErrorTextColor);
		tft.print(message);

		// copy the F() string to put it on the previousError stack.
		//this->previousError = strcpy_F(message);
		// TODO: ok, this is currently fuckin fucked, so I have no idea how I want to solve this stupid problem of not being able to make copies of F() strings, because C++ is stupid. How the hell are you supposed to be able to make a temporary value to hold a goddamn F() string so I can wipe and re-write this area of the goddamn display?!?!

		Serial.println(this->previousError);

		interrupts();
	}

	/// <summary>Prints sensor information to the screen.</summary>
	/// <param name="data">The <see cref="BME280Data"> containing readings from the sensors.</param>
	void ControllerDisplay::PrintSensors(BME280Data data)
	{
		noInterrupts();

		// NOTE: each of the method calls here first over-write the previous value with the background, and then the current value in the defined color for readings.

		this->PrintHumidity(&this->previousData, ControllerDisplay::BackgroundColor);
		this->PrintHumidity(&data, ControllerDisplay::ReadingsTextColor);

		this->PrintTemperature(&this->previousData, ControllerDisplay::BackgroundColor);
		this->PrintTemperature(&data, ControllerDisplay::ReadingsTextColor);

		interrupts();

		this->previousData = data;
	}

	void ControllerDisplay::PrintHumidity(BME280Data *data, uint16_t color)
	{
		tft.setFont(&FreeSansBold24pt7b);
		tft.setTextSize(1);
		char *humidity = BME280Data::ConvertFloatToString(data->Humidity, 4, 2);
		int16_t centeredTextXPosition = GetCenteredPosition(humidity, 0, 70, 150);
		tft.setCursor(centeredTextXPosition, 70);
		tft.setTextColor(color);
		tft.print(humidity);
	}

	void ControllerDisplay::PrintTemperature(BME280Data *data, uint16_t color)
	{
		tft.setFont(&FreeSansBold24pt7b);
		tft.setTextSize(1);
		char *temperature = BME280Data::ConvertFloatToString(data->Temperature, 4, 2);
		int16_t centeredTextXPosition = GetCenteredPosition(temperature, 0, 176, 150);
		tft.setCursor(centeredTextXPosition, 176);
		tft.setTextColor(color);
		tft.print(temperature);
	}

	int16_t ControllerDisplay::GetCenteredPosition(char *text, int16_t x, int16_t y, int16_t areaWidth)
	{
		// values used for centering text in display areas (rectangles)
		int16_t  x1, y1;
		uint16_t textWidth, textHeight;

		tft.getTextBounds(text, x, y, &x1, &y1, &textWidth, &textHeight);
		return ((areaWidth - textWidth) / 2) + x;
	}

	int16_t ControllerDisplay::GetCenteredPosition(const char *text, int16_t x, int16_t y, int16_t areaWidth)
	{
		// values used for centering text in display areas (rectangles)
		int16_t  x1, y1;
		uint16_t textWidth, textHeight;

		tft.getTextBounds(text, x, y, &x1, &y1, &textWidth, &textHeight);
		return ((areaWidth - textWidth) / 2) + x;
	}
}
