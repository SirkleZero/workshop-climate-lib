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
	const char* ControllerDisplay::ExampleOfALabel = "My Label";

	/// <summary>Initializes a new instance of the <see cref="ControllerDisplay"/> class.</summary>
	ControllerDisplay::ControllerDisplay() :
		tft(TFT_CS, TFT_DC),
		ts(STMPE_CS),
		humidityArea(160, 50, 160, 125),
		temperatureArea(0, 50, 160, 125),
		settingsButton(290, 0, 30, 30),
		homeButton(290, 0, 30, 30)
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

		this->touchscreenExists = ts.begin();

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

	void ControllerDisplay::Display()
	{
		// use the Touched() function to determine if a screen area has been pressed, and if so, where.
		this->Display(this->Touched());
	}

	void ControllerDisplay::Display(ScreenRegion region)
	{
		// Depending on which screen region needs to be displayed, update the display.
		// the call to touched will return ScreenRegion "none" as its default, since if
		// nothing was clicked on, well, that would make sense right? But, we don't want 
		// this Display() method to perpetually update the display, so we ignore values
		// of none, instead only focusing on if something was actually clicked in a
		// valid area.
		// We also check that actual data was updated which requires a redraw of the screen.
		this->selectedRegion = region;
		this->regionChanged = this->selectedRegion != ScreenRegion::None && this->selectedRegion != this->activeRegion;
		this->renderLayout = this->regionChanged; // TODO: render layout might be replaceable by region changed?
		if (this->DisplayUpdatable())
		{
			// nothing was actually clicked on, stay on the view we are currently rendering
			if (region != ScreenRegion::None)
			{
				this->activeRegion = this->selectedRegion;
			}

			switch (this->activeRegion)
			{
				case ScreenRegion::Home:
					Serial.println(F("ScreenRegion::Home"));
					this->DisplayHomeScreen();
					break;
				case ScreenRegion::Humidity:
					Serial.println(F("ScreenRegion::Humidity"));
					this->DisplayHumidityScreen();
					break;
				case ScreenRegion::Settings:
					Serial.println(F("ScreenRegion::Settings"));
					this->DisplaySettingsScreen();
					break;
				case ScreenRegion::Temperature:
					Serial.println(F("ScreenRegion::Temperature"));
					this->DisplayTemperatureScreen();
					break;
				default:
					Serial.println(F("default"));
					this->DisplayHomeScreen();
					break;
			}

			// once the display has been updated, reset the state variables that track change status
			this->DisplayUpdated();
		}
	}

	bool ControllerDisplay::DisplayUpdatable()
	{
		return this->dataChanged || this->regionChanged;
	}

	void ControllerDisplay::DisplayUpdated()
	{
		this->dataChanged = false;
		this->regionChanged = false;
	}

	/// <summary></summary>
	ScreenRegion ControllerDisplay::Touched()
	{
		if (!this->ts.bufferEmpty())
		{
			// Convert the point that was clicked to compensate for the screen orientation. While
			// the display knows it's orientation, the touchscreen does not.
			TS_Point p = this->ts.getPoint();
			int y = map(p.x, ControllerDisplay::TSMinY, ControllerDisplay::TSMaxY, 0, tft.height());
			int x = map(p.y, ControllerDisplay::TSMinX, ControllerDisplay::TSMaxX, 0, tft.width());

			Serial.print(F("x: ")); Serial.println(x);
			Serial.print(F("y: ")); Serial.println(y);

			if (this->humidityArea.Contains(x, y))
			{
				return ScreenRegion::Humidity;
			}
			if (this->temperatureArea.Contains(x, y))
			{
				return ScreenRegion::Temperature;
			}
			if (this->settingsButton.Contains(x, y))
			{
				return ScreenRegion::Home; // for testing only!
			}
			if (this->homeButton.Contains(x, y))
			{
				return ScreenRegion::Home;
			}
		}

		return ScreenRegion::None;
	}

	/// <summary>Prints an error message to the display.</summary>
	/// <param name="message">The message to display as an error.</param>
	void ControllerDisplay::LoadError(const __FlashStringHelper* message)
	{
		noInterrupts();

		tft.setFont();
		tft.setTextSize(1);

		char* errorLabel = "Error Happened!: ";

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
	void ControllerDisplay::LoadData(BME280Data data)
	{
		this->currentData = data;
		this->dataChanged = true;
	}

	/// <summary>Draws the initial static elements of the display.</summary>
	void ControllerDisplay::LayoutHomeScreen()
	{
		if (this->renderLayout)
		{
			noInterrupts();

			tft.fillScreen(ControllerDisplay::BackgroundColor);

			// for centering
			int16_t centeredTextXPosition;

			// draw our centerline
			tft.drawFastVLine(160, 30, 180, ControllerDisplay::LayoutLineColor);

			// for temperature
			char* temperatureLabel = "Fahrenheit";
			//tft.drawRect(temperatureArea.x, temperatureArea.y, temperatureArea.width, temperatureArea.height, ControllerDisplay::LayoutLineColor);
			tft.setFont(&FreeSansBold9pt7b);
			tft.setTextSize(1);
			centeredTextXPosition = GetCenteredPosition(temperatureLabel, temperatureArea.x, temperatureArea.y, temperatureArea.width);
			tft.setCursor(centeredTextXPosition, temperatureArea.y + temperatureArea.height - 5);
			tft.println(temperatureLabel);

			// for Humidity
			char* humidityLabel = "% Humidity";
			//tft.drawRect(humidityArea.x, humidityArea.y, humidityArea.width, humidityArea.height, ControllerDisplay::LayoutLineColor);
			tft.setFont(&FreeSansBold9pt7b);
			tft.setTextSize(1);
			centeredTextXPosition = GetCenteredPosition(humidityLabel, humidityArea.x, humidityArea.y, humidityArea.width);
			tft.setCursor(centeredTextXPosition, humidityArea.y + humidityArea.height - 5);
			tft.println(humidityLabel);

			// draw the box for the back to home button
			tft.drawRect(settingsButton.x, settingsButton.y, settingsButton.width, settingsButton.height, ControllerDisplay::LayoutLineColor);

			interrupts();

			this->renderLayout = false;
		}
	}

	void ControllerDisplay::LayoutHumidityScreen()
	{
		if (this->renderLayout)
		{
			tft.fillScreen(ILI9341_BLUE);

			this->renderLayout = false;
		}
	}

	void ControllerDisplay::LayoutTemperatureScreen()
	{
		if (this->renderLayout)
		{
			tft.fillScreen(ILI9341_RED);

			this->renderLayout = false;
		}
	}

	void ControllerDisplay::LayoutSettingsScreen()
	{
		if (this->renderLayout)
		{
			tft.fillScreen(ILI9341_GREEN);

			this->renderLayout = false;
		}
	}

	void ControllerDisplay::DisplayHomeScreen()
	{
		this->LayoutHomeScreen();

		noInterrupts();

		// NOTE: each of the method calls here first over-write the previous value with the background, and 
		// then the current value in the defined color for readings.
		if (this->IntegerPartChanged(this->previousData.Temperature, this->currentData.Temperature) || this->regionChanged)
		{
			this->PrintTemperature(&this->previousData, ControllerDisplay::BackgroundColor);
			this->PrintTemperature(&this->currentData, ControllerDisplay::ReadingsTextColor);
		}

		if (this->IntegerPartChanged(this->previousData.Humidity, this->currentData.Humidity) || this->regionChanged)
		{
			this->PrintHumidity(&this->previousData, ControllerDisplay::BackgroundColor);
			this->PrintHumidity(&this->currentData, ControllerDisplay::ReadingsTextColor);
		}

		interrupts();

		this->previousData = this->currentData;
	}

	void ControllerDisplay::DisplayHumidityScreen()
	{
		this->LayoutHumidityScreen();

		noInterrupts();
		
		interrupts();
	}

	void ControllerDisplay::DisplayTemperatureScreen()
	{
		this->LayoutTemperatureScreen();

		noInterrupts();
		
		interrupts();
	}

	void ControllerDisplay::DisplaySettingsScreen()
	{
		this->LayoutSettingsScreen();

		noInterrupts();
		
		interrupts();
	}

	void ControllerDisplay::PrintHumidity(BME280Data* data, uint16_t color)
	{
		tft.setFont(&FreeSansBold24pt7b);
		tft.setTextSize(2);
		char* humidity = BME280Data::ConvertFloatToString(data->Humidity, 2, 0);
		int16_t centeredTextXPosition = GetCenteredPosition(humidity, humidityArea.x, humidityArea.y, humidityArea.width);
		tft.setCursor(centeredTextXPosition, humidityArea.y + humidityArea.height - 35);
		tft.setTextColor(color, this->BackgroundColor); // apparently this doesn't work with custom fonts?
		tft.print(humidity);
	}

	void ControllerDisplay::PrintTemperature(BME280Data* data, uint16_t color)
	{
		tft.setFont(&FreeSansBold24pt7b);
		tft.setTextSize(2);
		char* temperature = BME280Data::ConvertFloatToString(data->Temperature, 2, 0);
		int16_t centeredTextXPosition = GetCenteredPosition(temperature, temperatureArea.x, temperatureArea.y, temperatureArea.width);
		tft.setCursor(centeredTextXPosition, temperatureArea.y + temperatureArea.height - 35);
		tft.setTextColor(color, this->BackgroundColor); // apparently this doesn't work with custom fonts?
		tft.print(temperature);
	}

	/// <summary>Prints the amount of free memory in bytes to the screen.</summary>
	/// <param name="freeMemory">The amount of free memory to display, in bytes.</param>
	void ControllerDisplay::PrintFreeMemory(int freeMemory)
	{
		noInterrupts();

		tft.setFont(&FreeSansBold9pt7b);
		tft.setTextSize(1);

		char* memoryLabel = "Free SRAM: ";

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

	bool ControllerDisplay::IntegerPartChanged(float first, float second)
	{
		/*double fractpart1, intpart1, fractpart2, intpart2;

		fractpart1 = modf(first, &intpart1);
		fractpart2 = modf(second, &intpart2);

		return intpart1 != intpart2;*/
		char* f = BME280Data::ConvertFloatToString(first, 2, 0);
		char* s = BME280Data::ConvertFloatToString(second, 2, 0);

		return strcmp(f, s) != 0;
	}

	int16_t ControllerDisplay::GetCenteredPosition(char* text, int16_t x, int16_t y, int16_t areaWidth)
	{
		// values used for centering text in display areas (rectangles)
		int16_t  x1, y1;
		uint16_t textWidth, textHeight;

		tft.getTextBounds(text, x, y, &x1, &y1, &textWidth, &textHeight);
		return ((areaWidth - textWidth) / 2) + x;
	}

	int16_t ControllerDisplay::GetCenteredPosition(const char* text, int16_t x, int16_t y, int16_t areaWidth)
	{
		// values used for centering text in display areas (rectangles)
		int16_t  x1, y1;
		uint16_t textWidth, textHeight;

		tft.getTextBounds(text, x, y, &x1, &y1, &textWidth, &textHeight);
		return ((areaWidth - textWidth) / 2) + x;
	}
}
