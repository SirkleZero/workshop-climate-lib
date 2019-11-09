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
	const char *ControllerDisplay::AqiScaleGoodLabel = "Good";
	const char *ControllerDisplay::AqiScaleModerateLabel = "Moderate";
	const char *ControllerDisplay::AqiScaleUnhealthySensitiveLabel = "Sensitive";
	const char *ControllerDisplay::AqiScaleUnhealthyLabel = "Unhealthy";
	const char *ControllerDisplay::AqiScaleVeryUnhealthyLabel = "Very Unhealthy";
	const char *ControllerDisplay::AqiScaleHazardousLabel = "Hazardous";

	/// <summary>Initializes a new instance of the <see cref="ControllerDisplay"/> class.</summary>
	ControllerDisplay::ControllerDisplay() :
		tft(TFT_CS, TFT_DC),
		humidityArea(0, 0, 150, 106),
		temperatureArea(0, 106, 150, 106),
		particulateArea(150, 28, 170, 184),
		aqiLabelArea(150, 0, 170, 28),
		aqiIndicatorArea(0, 212, 320, 28),
		aqiScaleGoodArea(160, 216, 25, 20, ControllerDisplay::AqiScaleGoodLabel, ControllerDisplay::AqiGood, 0, 50),
		aqiScaleModerateArea(185, 216, 25, 20, ControllerDisplay::AqiScaleModerateLabel, ControllerDisplay::AqiModerate, 51, 100),
		aqiScaleUnhealthySensitiveArea(210, 216, 25, 20, ControllerDisplay::AqiScaleUnhealthySensitiveLabel, ControllerDisplay::AqiUnhealthySensitive, 101, 150),
		aqiScaleUnhealthyArea(235, 216, 25, 20, ControllerDisplay::AqiScaleUnhealthyLabel, ControllerDisplay::AqiUnhealthy, 151, 200),
		aqiScaleVeryUnhealthyArea(260, 216, 25, 20, ControllerDisplay::AqiScaleVeryUnhealthyLabel, ControllerDisplay::AqiVeryUnhealthy, 201, 300),
		aqiScaleHazardousArea(285, 216, 25, 20, ControllerDisplay::AqiScaleHazardousLabel, ControllerDisplay::AqiHazardous, 301, 500),
		aqiScaleStrokeArea(159, 216, 152, 20)
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

		// for AQI Label
		char *AQILabel = "AQI (ug/m3)";
		tft.drawRect(aqiLabelArea.x, aqiLabelArea.y, aqiLabelArea.width, aqiLabelArea.height, ControllerDisplay::LayoutLineColor);
		tft.setFont(&FreeSansBold9pt7b);
		tft.setTextSize(1);
		centeredTextXPosition = GetCenteredPosition(AQILabel, 150, 18, 170);
		tft.setCursor(centeredTextXPosition, 18);
		tft.println(AQILabel);

		// for particulates area
		tft.drawRect(particulateArea.x, particulateArea.y, particulateArea.width, particulateArea.height, ControllerDisplay::LayoutLineColor);

		// for AQI Indicator
		tft.drawRect(aqiIndicatorArea.x, aqiIndicatorArea.y, aqiIndicatorArea.width, aqiIndicatorArea.height, ControllerDisplay::LayoutLineColor);

		// air quality scale
		tft.fillRect(aqiScaleGoodArea.x, aqiScaleGoodArea.y, aqiScaleGoodArea.width, aqiScaleGoodArea.height, ControllerDisplay::AqiGood);
		tft.fillRect(aqiScaleModerateArea.x, aqiScaleModerateArea.y, aqiScaleModerateArea.width, aqiScaleModerateArea.height, ControllerDisplay::AqiModerate);
		tft.fillRect(aqiScaleUnhealthySensitiveArea.x, aqiScaleUnhealthySensitiveArea.y, aqiScaleUnhealthySensitiveArea.width, aqiScaleUnhealthySensitiveArea.height, ControllerDisplay::AqiUnhealthySensitive);
		tft.fillRect(aqiScaleUnhealthyArea.x, aqiScaleUnhealthyArea.y, aqiScaleUnhealthyArea.width, aqiScaleUnhealthyArea.height, ControllerDisplay::AqiUnhealthy);
		tft.fillRect(aqiScaleVeryUnhealthyArea.x, aqiScaleVeryUnhealthyArea.y, aqiScaleVeryUnhealthyArea.width, aqiScaleVeryUnhealthyArea.height, ControllerDisplay::AqiVeryUnhealthy);
		tft.fillRect(aqiScaleHazardousArea.x, aqiScaleHazardousArea.y, aqiScaleHazardousArea.width, aqiScaleHazardousArea.height, ControllerDisplay::AqiHazardous);

		// stroke the scale
		tft.drawRect(aqiScaleStrokeArea.x, aqiScaleStrokeArea.y, aqiScaleStrokeArea.width, aqiScaleStrokeArea.height, ControllerDisplay::LayoutLineColor);

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

		/*this->PrintParticulates(&this->previousData, ControllerDisplay::BackgroundColor);
		this->PrintParticulates(&data, ControllerDisplay::ReadingsTextColor);

		this->DrawAirQualityIndicator(&this->previousData, true);
		this->DrawAirQualityIndicator(&data, false);*/

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

	//void ControllerDisplay::PrintParticulates(SensorData *data, uint16_t color)
	//{
	//	//tft.setFont(&FreeSans9pt7b);
	//	tft.setFont();
	//	tft.setTextSize(1);

	//	// TODO: break this apart into a portion that will go into the layout method
	//	// and a portion (here) responsible for drawing out the values of the sensor.

	//	tft.setTextColor(ControllerDisplay::LayoutTextColor);
	//	tft.setCursor(156, 40);
	//	tft.print("Standard");

	//	tft.setTextColor(color);
	//	tft.setCursor(156, 53);
	//	tft.print("PM1.0:");
	//	tft.setCursor(200, 53);
	//	tft.print(data->Particulates.pm10_standard);

	//	tft.setCursor(156, 68);
	//	tft.print("PM2.5:");
	//	tft.setCursor(200, 68);
	//	tft.print(data->Particulates.pm25_standard);

	//	tft.setCursor(156, 83);
	//	tft.print("PM10.0:");
	//	tft.setCursor(200, 83);
	//	tft.print(data->Particulates.pm100_standard);

	//	tft.setTextColor(ControllerDisplay::LayoutTextColor);
	//	tft.setCursor(235, 40);
	//	tft.print("Environmental");

	//	tft.setTextColor(color);
	//	tft.setCursor(235, 53);
	//	tft.print("PM1.0:");
	//	tft.setCursor(279, 53);
	//	tft.print(data->Particulates.pm10_env);

	//	tft.setCursor(235, 68);
	//	tft.print("PM2.5:");
	//	tft.setCursor(279, 68);
	//	tft.print(data->Particulates.pm25_env);

	//	tft.setCursor(235, 83);
	//	tft.print("PM10.0:");
	//	tft.setCursor(279, 83);
	//	tft.print(data->Particulates.pm100_env);
	//}

	//void ControllerDisplay::DrawAirQualityIndicator(SensorData *data, bool overwritting)
	//{
	//	uint16_t aqi = data->Particulates.pm25_standard;
	//	uint16_t color;
	//	uint16_t indicatorColor; // the color of the verticle indicator
	//	int16_t indicatorXPosition = 0; // the verticle indicator line's x position
	//	int16_t centeredTextXPosition; // for centering

	//	AqiScaleRectangle *tempScaleArea;

	//	// if the aqi is greater than 500, set it to 500. The AQI scale only goes to 500 so having it be larger would be pretty much meaningless. If the scale of the index where to change, then the settings for all of this would need to change to compensate for the new scale.
	//	if (aqi > ControllerDisplay::AqiScaleMaximum)
	//	{
	//		aqi = ControllerDisplay::AqiScaleMaximum;
	//	}

	//	if (aqiScaleGoodArea.IsAqiRectangleFor(aqi))
	//	{
	//		tempScaleArea = &aqiScaleGoodArea;
	//	}
	//	else if (aqiScaleModerateArea.IsAqiRectangleFor(aqi))
	//	{
	//		tempScaleArea = &aqiScaleModerateArea;
	//	}
	//	else if (aqiScaleUnhealthySensitiveArea.IsAqiRectangleFor(aqi))
	//	{
	//		tempScaleArea = &aqiScaleUnhealthySensitiveArea;
	//	}
	//	else if (aqiScaleUnhealthyArea.IsAqiRectangleFor(aqi))
	//	{
	//		tempScaleArea = &aqiScaleUnhealthyArea;
	//	}
	//	else if (aqiScaleVeryUnhealthyArea.IsAqiRectangleFor(aqi))
	//	{
	//		tempScaleArea = &aqiScaleVeryUnhealthyArea;
	//	}
	//	else if (aqiScaleHazardousArea.IsAqiRectangleFor(aqi))
	//	{
	//		tempScaleArea = &aqiScaleHazardousArea;
	//	}
	//	else
	//	{
	//		tempScaleArea = &aqiScaleHazardousArea;
	//	}

	//	color = tempScaleArea->Color;
	//	indicatorXPosition = tempScaleArea->GetVerticalIndicatorLocation(aqi);

	//	if (overwritting)
	//	{
	//		indicatorColor = color;
	//		color = BackgroundColor;
	//	}
	//	else
	//	{
	//		indicatorColor = BackgroundColor;
	//	}

	//	// print out the friendly AQI index name
	//	tft.setFont(&FreeSans9pt7b);
	//	tft.setTextSize(1);
	//	tft.setTextColor(color);
	//	centeredTextXPosition = GetCenteredPosition(tempScaleArea->Label, 0, 231, 160);
	//	tft.setCursor(centeredTextXPosition, 231);
	//	tft.println(tempScaleArea->Label);

	//	// draw an indicator line on the scale. 160 is the beginning of the scale, and we increment up from that. Only draw the indicator line if we fit within our aqi scale of 0 to 500.
	//	if (aqi > 0 && aqi <= 500)
	//	{
	//		tft.drawFastVLine(indicatorXPosition, 217, 18, indicatorColor);
	//	}
	//}

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
