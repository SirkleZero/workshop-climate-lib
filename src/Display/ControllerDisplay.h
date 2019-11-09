#ifndef ControllerDisplay_h
#define ControllerDisplay_h

// external dependencies
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>      // this is needed even tho we aren't using it
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>

// include some fonts
#include <Fonts\FreeSansBold24pt7b.h>
//#include <Fonts\FreeSansBold12pt7b.h>
#include <Fonts\FreeSansBold9pt7b.h>
//#include <Fonts\FreeSans9pt7b.h>

#include "..\InitializationResult.h"
#include "..\Sensors\BME280Data.h"
#include "..\Rectangle.h"
#include "TouchScreenRegion.h"

using namespace Sensors;

namespace Display {
	/// <summary>Defines a class that controls the display of the controller unit.</summary>
	class ControllerDisplay
	{
	public:
		ControllerDisplay();
		InitializationResult Initialize();
		void PrintError(const __FlashStringHelper *message);
		void PrintSensors(BME280Data data);
		void PrintFreeMemory(int freeMemory);
		void DrawLayout();
		void Clear();
		TouchScreenRegion Touched();
	private:
		// a constant that defines the rotational position of the display.
		// this could become dynamic if we used an accelerometer to determine
		// the position of the display unit.
		static const uint8_t ScreenOrientation = 1;

		// Interface color constants
		static const uint16_t ReadingsTextColor = 0x5DDF;
		static const uint16_t LayoutTextColor = ILI9341_WHITE;
		static const uint16_t ErrorTextColor = ILI9341_YELLOW;
		static const uint16_t LayoutLineColor = ILI9341_WHITE;
		static const uint16_t BackgroundColor = ILI9341_BLACK;

		// Interface label constants
		static const char *ExampleOfALabel;

		// Display unit variables
		Adafruit_ILI9341 tft;
		Adafruit_STMPE610 ts;
		uint16_t height;
		uint16_t width;
		
		// variables used for display state management
		BME280Data previousData;
		int previousFreeMemory = 0;
		char *previousError;

		// constants and variables for the touchscreen
		bool touchscreenExists = false;
		static const uint16_t TSMinX = 150;
		static const uint16_t TSMinY = 130;
		static const uint16_t TSMaxX = 3800;
		static const uint16_t TSMaxY = 4000;

		// Define the rectangles that are used to draw the screen layout elements
		Rectangle humidityArea;
		Rectangle temperatureArea;

		// private functions
		void PrintTemperature(BME280Data *data, uint16_t color);
		void PrintHumidity(BME280Data *data, uint16_t color);
		int16_t GetCenteredPosition(char *text, int16_t x, int16_t y, int16_t areaWidth);
		int16_t GetCenteredPosition(const char *text, int16_t x, int16_t y, int16_t areaWidth);
	};
}

#endif
