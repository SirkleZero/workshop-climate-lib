#ifndef RXTFTFeatherwingProxy_h
#define RXTFTFeatherwingProxy_h

// external dependencies
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// include some fonts
#include <Fonts\FreeSansBold24pt7b.h>
#include <Fonts\FreeSansBold12pt7b.h>
#include <Fonts\FreeSansBold9pt7b.h>
#include <Fonts\FreeSans9pt7b.h>

#include "..\InitializationResult.h"
#include "..\Sensors\SensorData.h"
#include "..\Rectangle.h"

using namespace Sensors;

namespace Display {
	/// <summary>Defines a class that controls the display of the controller unit.</summary>
	class RXTFTFeatherwingProxy {
	public:
		RXTFTFeatherwingProxy();
		InitializationResult Initialize();
		void PrintSensors(SensorData data);
		void PrintFreeMemory(int freeMemory);
		void DrawLayout();
		void Clear();
	private:
		// Constants
		static const int AqiScaleMaximum = 500;

		// Interface color constants
		static const uint16_t ReadingsTextColor = 0x5DDF;
		static const uint16_t LayouttextColor = ILI9341_WHITE;
		static const uint16_t LayoutlineColor = ILI9341_WHITE;
		static const uint16_t BackgroundColor = ILI9341_BLACK;
		static const uint16_t AqiGood = ILI9341_GREEN;
		static const uint16_t AqiModerate = ILI9341_YELLOW;
		static const uint16_t AqiUnhealthySensitive = ILI9341_ORANGE;
		static const uint16_t AqiUnhealthy = ILI9341_RED;
		static const uint16_t AqiVeryUnhealthy = 0xB800;
		static const uint16_t AqiHazardous = 0x8000;

		// Interface label constants
		static const char *AqiScaleGoodLabel;
		static const char *AqiScaleModerateLabel;
		static const char *AqiScaleUnhealthySensitiveLabel;
		static const char *AqiScaleUnhealthyLabel;
		static const char *AqiScaleVeryUnhealthyLabel;
		static const char *AqiScaleHazardousLabel;

		// Display unit variables
		Adafruit_ILI9341 tft;
		uint16_t height;
		uint16_t width;

		// a constant that defines the rotational position of the display.
		// this could become dynamic if we used an accelerometer to determine
		// the position of the display unit.
		static const uint8_t ScreenOrientation = 1;
		
		// variables used for display state management
		SensorData previousData;
		int previousFreeMemory = 0;

		// Define the rectangles that are used to draw the screen layout elements
		Rectangle humidityArea;
		Rectangle temperatureArea;
		Rectangle particulateArea;
		Rectangle aqiLabelArea;
		Rectangle aqiIndicatorArea;
		AqiScaleRectangle aqiScaleGoodArea;
		AqiScaleRectangle aqiScaleModerateArea;
		AqiScaleRectangle aqiScaleUnhealthySensitiveArea;
		AqiScaleRectangle aqiScaleUnhealthyArea;
		AqiScaleRectangle aqiScaleVeryUnhealthyArea;
		AqiScaleRectangle aqiScaleHazardousArea;
		Rectangle aqiScaleStrokeArea;

		// private functions
		void PrintTemperature(SensorData *data, uint16_t color);
		void PrintHumidity(SensorData *data, uint16_t color);
		void PrintParticulates(SensorData *data, uint16_t color);
		int16_t GetCenteredPosition(char *text, int16_t x, int16_t y, int16_t areaWidth);
		int16_t GetCenteredPosition(const char *text, int16_t x, int16_t y, int16_t areaWidth);
		void DrawAirQualityIndicator(SensorData *data, bool overwritting);
	};
}

#endif
