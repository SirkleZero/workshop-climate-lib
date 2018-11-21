#include "RXTFTFeatherwingProxy.h"

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
	const char *RXTFTFeatherwingProxy::AqiScaleGoodLabel = "Good";
	const char *RXTFTFeatherwingProxy::AqiScaleModerateLabel = "Moderate";
	const char *RXTFTFeatherwingProxy::AqiScaleUnhealthySensitiveLabel = "Sensitive";
	const char *RXTFTFeatherwingProxy::AqiScaleUnhealthyLabel = "Unhealthy";
	const char *RXTFTFeatherwingProxy::AqiScaleVeryUnhealthyLabel = "Very Unhealthy";
	const char *RXTFTFeatherwingProxy::AqiScaleHazardousLabel = "Hazardous";

	/// <summary>Initializes a new instance of the <see cref="RXTFTFeatherwingProxy"/> class.</summary>
	RXTFTFeatherwingProxy::RXTFTFeatherwingProxy() :
		tft(TFT_CS, TFT_DC),
		humidityArea(0, 0, 150, 106),
		temperatureArea(0, 106, 150, 106),
		particulateArea(150, 28, 170, 184),
		aqiLabelArea(150, 0, 170, 28),
		aqiIndicatorArea(0, 212, 320, 28),
		aqiScaleGoodArea(160, 216, 25, 20, RXTFTFeatherwingProxy::AqiScaleGoodLabel, RXTFTFeatherwingProxy::AqiGood, 0, 50),
		aqiScaleModerateArea(185, 216, 25, 20, RXTFTFeatherwingProxy::AqiScaleModerateLabel, RXTFTFeatherwingProxy::AqiModerate, 51, 100),
		aqiScaleUnhealthySensitiveArea(210, 216, 25, 20, RXTFTFeatherwingProxy::AqiScaleUnhealthySensitiveLabel, RXTFTFeatherwingProxy::AqiUnhealthySensitive, 101, 150),
		aqiScaleUnhealthyArea(235, 216, 25, 20, RXTFTFeatherwingProxy::AqiScaleUnhealthyLabel, RXTFTFeatherwingProxy::AqiUnhealthy, 151, 200),
		aqiScaleVeryUnhealthyArea(260, 216, 25, 20, RXTFTFeatherwingProxy::AqiScaleVeryUnhealthyLabel, RXTFTFeatherwingProxy::AqiVeryUnhealthy, 201, 300),
		aqiScaleHazardousArea(285, 216, 25, 20, RXTFTFeatherwingProxy::AqiScaleHazardousLabel, RXTFTFeatherwingProxy::AqiHazardous, 301, 500),
		aqiScaleStrokeArea(159, 216, 152, 20)
	{}

	/// <summary>Executes initialization logic for the object.</summary>
	/// <returns>An <see cref="InitializationResult"/> that describes the result of initialization.</returns>
	InitializationResult RXTFTFeatherwingProxy::Initialize()
	{
		InitializationResult result;

		tft.begin();

		noInterrupts();
		tft.setRotation(RXTFTFeatherwingProxy::ScreenOrientation);
		interrupts();

		this->height = tft.height();
		this->width = tft.width();

		result.IsSuccessful = true;
		return result;
	}

	/// <summary>Clears all displayed content from the screen by wiping it with a black background.</summary>
	void RXTFTFeatherwingProxy::Clear()
	{
		noInterrupts();
		tft.fillScreen(RXTFTFeatherwingProxy::BackgroundColor);
		interrupts();
	}

	/// <summary>Draws the initial static elements of the display.</summary>
	void RXTFTFeatherwingProxy::DrawLayout()
	{
		noInterrupts();

		// for centering
		int16_t centeredTextXPosition;

		// for Humidity
		char *humidityLabel = "% Humidity";
		tft.drawRect(humidityArea.x, humidityArea.y, humidityArea.width, humidityArea.height, RXTFTFeatherwingProxy::LayoutlineColor);
		tft.setFont(&FreeSansBold9pt7b);
		tft.setTextSize(1);
		centeredTextXPosition = GetCenteredPosition(humidityLabel, 0, 90, 150);
		tft.setCursor(centeredTextXPosition, 90);
		tft.println(humidityLabel);

		// for temperature
		char *temperatureLabel = "Fahrenheit";
		tft.drawRect(temperatureArea.x, temperatureArea.y, temperatureArea.width, temperatureArea.height, RXTFTFeatherwingProxy::LayoutlineColor);
		tft.setFont(&FreeSansBold9pt7b);
		tft.setTextSize(1);
		centeredTextXPosition = GetCenteredPosition(temperatureLabel, 0, 196, 150);
		tft.setCursor(centeredTextXPosition, 196);
		tft.println(temperatureLabel);

		// for AQI Label
		char *AQILabel = "AQI (ug/m3)";
		tft.drawRect(aqiLabelArea.x, aqiLabelArea.y, aqiLabelArea.width, aqiLabelArea.height, RXTFTFeatherwingProxy::LayoutlineColor);
		tft.setFont(&FreeSansBold9pt7b);
		tft.setTextSize(1);
		centeredTextXPosition = GetCenteredPosition(AQILabel, 150, 18, 170);
		tft.setCursor(centeredTextXPosition, 18);
		tft.println(AQILabel);

		// for particulates area
		tft.drawRect(particulateArea.x, particulateArea.y, particulateArea.width, particulateArea.height, RXTFTFeatherwingProxy::LayoutlineColor);

		// for AQI Indicator
		tft.drawRect(aqiIndicatorArea.x, aqiIndicatorArea.y, aqiIndicatorArea.width, aqiIndicatorArea.height, RXTFTFeatherwingProxy::LayoutlineColor);

		// air quality scale
		tft.fillRect(aqiScaleGoodArea.x, aqiScaleGoodArea.y, aqiScaleGoodArea.width, aqiScaleGoodArea.height, RXTFTFeatherwingProxy::AqiGood);
		tft.fillRect(aqiScaleModerateArea.x, aqiScaleModerateArea.y, aqiScaleModerateArea.width, aqiScaleModerateArea.height, RXTFTFeatherwingProxy::AqiModerate);
		tft.fillRect(aqiScaleUnhealthySensitiveArea.x, aqiScaleUnhealthySensitiveArea.y, aqiScaleUnhealthySensitiveArea.width, aqiScaleUnhealthySensitiveArea.height, RXTFTFeatherwingProxy::AqiUnhealthySensitive);
		tft.fillRect(aqiScaleUnhealthyArea.x, aqiScaleUnhealthyArea.y, aqiScaleUnhealthyArea.width, aqiScaleUnhealthyArea.height, RXTFTFeatherwingProxy::AqiUnhealthy);
		tft.fillRect(aqiScaleVeryUnhealthyArea.x, aqiScaleVeryUnhealthyArea.y, aqiScaleVeryUnhealthyArea.width, aqiScaleVeryUnhealthyArea.height, RXTFTFeatherwingProxy::AqiVeryUnhealthy);
		tft.fillRect(aqiScaleHazardousArea.x, aqiScaleHazardousArea.y, aqiScaleHazardousArea.width, aqiScaleHazardousArea.height, RXTFTFeatherwingProxy::AqiHazardous);

		// stroke the scale
		tft.drawRect(aqiScaleStrokeArea.x, aqiScaleStrokeArea.y, aqiScaleStrokeArea.width, aqiScaleStrokeArea.height, RXTFTFeatherwingProxy::LayoutlineColor);

		interrupts();
	}

	/// <summary>Prints the amount of free memory in bytes to the screen.</summary>
	/// <param name="freeMemory">The amount of free memory to display, in bytes.</param>
	void RXTFTFeatherwingProxy::PrintFreeMemory(int freeMemory)
	{
		noInterrupts();

		tft.setFont(&FreeSansBold9pt7b);
		tft.setTextSize(1);

		char *memoryLabel = "Free SRAM: ";

		// overwrite
		tft.setCursor(156, 120);
		tft.setTextColor(RXTFTFeatherwingProxy::BackgroundColor);
		tft.print(memoryLabel);
		tft.print(previousFreeMemory);

		// print the value
		tft.setCursor(156, 120);
		tft.setTextColor(RXTFTFeatherwingProxy::ReadingsTextColor);
		tft.print(memoryLabel);
		tft.print(freeMemory);

		this->previousFreeMemory = freeMemory;

		interrupts();
	}

	/// <summary>Prints sensor information to the screen.</summary>
	/// <param name="data">The <see cref="SensorData"> containing readings from the sensors.</param>
	void RXTFTFeatherwingProxy::PrintSensors(SensorData data)
	{
		noInterrupts();

		// NOTE: each of the method calls here first over-write the previous value with the background, and then the current value in the defined color for readings.

		this->PrintHumidity(&this->previousData, RXTFTFeatherwingProxy::BackgroundColor);
		this->PrintHumidity(&data, RXTFTFeatherwingProxy::ReadingsTextColor);

		this->PrintTemperature(&this->previousData, RXTFTFeatherwingProxy::BackgroundColor);
		this->PrintTemperature(&data, RXTFTFeatherwingProxy::ReadingsTextColor);

		this->PrintParticulates(&this->previousData, RXTFTFeatherwingProxy::BackgroundColor);
		this->PrintParticulates(&data, RXTFTFeatherwingProxy::ReadingsTextColor);

		this->DrawAirQualityIndicator(&this->previousData, true);
		this->DrawAirQualityIndicator(&data, false);

		interrupts();

		this->previousData = data;
	}

	void RXTFTFeatherwingProxy::PrintHumidity(SensorData *data, uint16_t color)
	{
		tft.setFont(&FreeSansBold24pt7b);
		tft.setTextSize(1);
		char *humidity = ClimateData::ConvertFloatToString(data->Climate.Humidity, 4, 2);
		int16_t centeredTextXPosition = GetCenteredPosition(humidity, 0, 70, 150);
		tft.setCursor(centeredTextXPosition, 70);
		tft.setTextColor(color);
		tft.print(humidity);
	}

	void RXTFTFeatherwingProxy::PrintTemperature(SensorData *data, uint16_t color)
	{
		tft.setFont(&FreeSansBold24pt7b);
		tft.setTextSize(1);
		char *temperature = ClimateData::ConvertFloatToString(data->Climate.Temperature, 4, 2);
		int16_t centeredTextXPosition = GetCenteredPosition(temperature, 0, 176, 150);
		tft.setCursor(centeredTextXPosition, 176);
		tft.setTextColor(color);
		tft.print(temperature);
	}

	void RXTFTFeatherwingProxy::PrintParticulates(SensorData *data, uint16_t color)
	{
		//tft.setFont(&FreeSans9pt7b);
		tft.setFont();
		tft.setTextSize(1);

		// TODO: break this apart into a portion that will go into the layout method
		// and a portion (here) responsible for drawing out the values of the sensor.

		tft.setTextColor(RXTFTFeatherwingProxy::LayouttextColor);
		tft.setCursor(156, 40);
		tft.print("Standard");

		tft.setTextColor(color);
		tft.setCursor(156, 53);
		tft.print("PM1.0:");
		tft.setCursor(200, 53);
		tft.print(data->Particulates.pm10_standard);

		tft.setCursor(156, 68);
		tft.print("PM2.5:");
		tft.setCursor(200, 68);
		tft.print(data->Particulates.pm25_standard);

		tft.setCursor(156, 83);
		tft.print("PM10.0:");
		tft.setCursor(200, 83);
		tft.print(data->Particulates.pm100_standard);

		tft.setTextColor(RXTFTFeatherwingProxy::LayouttextColor);
		tft.setCursor(235, 40);
		tft.print("Environmental");

		tft.setTextColor(color);
		tft.setCursor(235, 53);
		tft.print("PM1.0:");
		tft.setCursor(279, 53);
		tft.print(data->Particulates.pm10_env);

		tft.setCursor(235, 68);
		tft.print("PM2.5:");
		tft.setCursor(279, 68);
		tft.print(data->Particulates.pm25_env);

		tft.setCursor(235, 83);
		tft.print("PM10.0:");
		tft.setCursor(279, 83);
		tft.print(data->Particulates.pm100_env);
	}

	void RXTFTFeatherwingProxy::DrawAirQualityIndicator(SensorData *data, bool overwritting)
	{
		uint16_t aqi = data->Particulates.pm25_standard;
		uint16_t color;
		uint16_t indicatorColor; // the color of the verticle indicator
		int16_t indicatorXPosition = 0; // the verticle indicator line's x position
		int16_t centeredTextXPosition; // for centering

		AqiScaleRectangle *tempScaleArea;

		// if the aqi is greater than 500, set it to 500. The AQI scale only goes to 500 so having it be larger would be pretty much meaningless. If the scale of the index where to change, then the settings for all of this would need to change to compensate for the new scale.
		if (aqi > RXTFTFeatherwingProxy::AqiScaleMaximum)
		{
			aqi = RXTFTFeatherwingProxy::AqiScaleMaximum;
		}

		if (aqiScaleGoodArea.IsAqiRectangleFor(aqi))
		{
			tempScaleArea = &aqiScaleGoodArea;
		}
		else if (aqiScaleModerateArea.IsAqiRectangleFor(aqi))
		{
			tempScaleArea = &aqiScaleModerateArea;
		}
		else if (aqiScaleUnhealthySensitiveArea.IsAqiRectangleFor(aqi))
		{
			tempScaleArea = &aqiScaleUnhealthySensitiveArea;
		}
		else if (aqiScaleUnhealthyArea.IsAqiRectangleFor(aqi))
		{
			tempScaleArea = &aqiScaleUnhealthyArea;
		}
		else if (aqiScaleVeryUnhealthyArea.IsAqiRectangleFor(aqi))
		{
			tempScaleArea = &aqiScaleVeryUnhealthyArea;
		}
		else if (aqiScaleHazardousArea.IsAqiRectangleFor(aqi))
		{
			tempScaleArea = &aqiScaleHazardousArea;
		}
		else
		{
			tempScaleArea = &aqiScaleHazardousArea;
		}

		color = tempScaleArea->Color;
		indicatorXPosition = tempScaleArea->GetVerticalIndicatorLocation(aqi);

		if (overwritting)
		{
			indicatorColor = color;
			color = BackgroundColor;
		}
		else
		{
			indicatorColor = BackgroundColor;
		}

		// print out the friendly AQI index name
		tft.setFont(&FreeSans9pt7b);
		tft.setTextSize(1);
		tft.setTextColor(color);
		centeredTextXPosition = GetCenteredPosition(tempScaleArea->Label, 0, 231, 160);
		tft.setCursor(centeredTextXPosition, 231);
		tft.println(tempScaleArea->Label);

		// draw an indicator line on the scale. 160 is the beginning of the scale, and we increment up from that. Only draw the indicator line if we fit within our aqi scale of 0 to 500.
		if (aqi > 0 && aqi <= 500)
		{
			tft.drawFastVLine(indicatorXPosition, 217, 18, indicatorColor);
		}
	}

	int16_t RXTFTFeatherwingProxy::GetCenteredPosition(char *text, int16_t x, int16_t y, int16_t areaWidth)
	{
		// values used for centering text in display areas (rectangles)
		int16_t  x1, y1;
		uint16_t textWidth, textHeight;

		tft.getTextBounds(text, x, y, &x1, &y1, &textWidth, &textHeight);
		return ((areaWidth - textWidth) / 2) + x;
	}

	int16_t RXTFTFeatherwingProxy::GetCenteredPosition(const char *text, int16_t x, int16_t y, int16_t areaWidth)
	{
		// values used for centering text in display areas (rectangles)
		int16_t  x1, y1;
		uint16_t textWidth, textHeight;

		tft.getTextBounds(text, x, y, &x1, &y1, &textWidth, &textHeight);
		return ((areaWidth - textWidth) / 2) + x;
	}
}
