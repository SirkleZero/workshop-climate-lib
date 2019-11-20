#ifndef MonitorDisplay_h
#define MonitorDisplay_h

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

// external dependencies
#include <Arduino.h>
#include <math.h>
#include <SPI.h>
#include <Wire.h>      // this is needed even tho we aren't using it
#include <Adafruit_GFX.h>
#include <Adafruit_HX8357.h>
#include <Adafruit_STMPE610.h>
#include <MemoryFree.h>

// include some fonts
#include "Fonts\bahnschrift90pt7b.h"
#include "Fonts\calibrib12pt7b.h"

#include "..\InitializationResult.h"
#include "..\Sensors\BME280Data.h"
#include "..\Rectangle.h"
#include "ButtonRectangle.h"
#include "ScreenRegion.h"

using namespace Sensors;

namespace Display {
	/// <summary>Defines a class that controls the display of the monitor unit.</summary>
	class MonitorDisplay
	{
	public:
		MonitorDisplay();
		InitializationResult Initialize();
		void LoadError(const __FlashStringHelper* message);
		void LoadMessage(const __FlashStringHelper* message);
		void LoadSensorData(BME280Data data);
		void Display();
		void Display(ScreenRegion region);
		void Clear();
		ScreenRegion Touched();
	private:
		// a constant that defines the rotational position of the display.
		static const uint8_t ScreenOrientation = 1;

		// Interface color constants
		static const uint16_t ReadingsTextColor = 0x5DDF;
		static const uint16_t LayoutTextColor = HX8357_WHITE;
		static const uint16_t ErrorTextColor = HX8357_YELLOW;
		static const uint16_t LayoutLineColor = HX8357_WHITE;
		static const uint16_t BackgroundColor = HX8357_BLACK;

		// Interface label constants
		static const char* ExampleOfALabel;

		// Display unit variables
		Adafruit_HX8357 tft;
		Adafruit_STMPE610 ts;
		uint16_t height;
		uint16_t width;

		// variables used for display state management
		bool dataChanged = false;
		bool regionChanged = false;
		BME280Data currentBME280Data;
		BME280Data previousBME280Data;
		const __FlashStringHelper* currentMessage;
		const __FlashStringHelper* previousMessage;
		int previousFreeMemory = 0;
		char* previousError;

		// constants and variables for the touchscreen
		bool touchscreenExists = false;
		ScreenRegion selectedRegion = ScreenRegion::None;
		ScreenRegion activeRegion = ScreenRegion::None;
		// these constants are used to calculate the offsets of the touchscreen
		static const uint16_t TSMinX = 150;
		static const uint16_t TSMinY = 130;
		static const uint16_t TSMaxX = 3800;
		static const uint16_t TSMaxY = 4000;

		// Define the rectangles that are used to draw the screen layout elements
		ButtonRectangle humidityArea;
		ButtonRectangle temperatureArea;
		ButtonRectangle settingsButton;
		ButtonRectangle homeButton;
		Rectangle centeredMessageBox;

		// private functions
		void LayoutHomeScreen();
		void DisplayHomeScreen();

		void LayoutHumidityScreen();
		void DisplayHumidityScreen();
		void PrintHumidity(BME280Data* data, uint16_t color);

		void LayoutTemperatureScreen();
		void DisplayTemperatureScreen();
		void PrintTemperature(BME280Data* data, uint16_t color);

		void LayoutMessageScreen();
		void DisplayMessageScreen();
		void PrintMessage();

		void LayoutSettingsScreen();
		void DisplaySettingsScreen();

		void PrintFreeMemory(int freeMemory);

		void ConfigureButtons(ScreenRegion region);
		bool DisplayUpdatable();
		void DisplayUpdated();
		bool IntegerPartChanged(float first, float second);

		int16_t GetCenteredPosition(char* text, int16_t x, int16_t y, int16_t areaWidth);
		int16_t GetCenteredPosition(const char* text, int16_t x, int16_t y, int16_t areaWidth);
		int16_t GetCenteredPosition(const __FlashStringHelper* text, int16_t x, int16_t y, int16_t areaWidth);
	};
}

#endif
