#pragma once

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "..\InitializationResult.h"
#include "..\Sensors\SensorData.h"
#include "..\TX\TXResult.h"

using namespace Sensors;
using namespace TX;

namespace Display {
	/// <summary>Describes the state of the buttons of the Feather OLED display.</summary>
	enum ButtonMode {
		Default = 1u << 0,
		A = 1u << 1,
		B = 1u << 2,
		C = 1u << 3
	};

	/// <summary>Defines a class that represents the display of the Sensor module.</summary>
	class FeatherOLEDProxy {
	public:
		FeatherOLEDProxy();

		InitializationResult Initialize();
		void PrintSensors(SensorData data);
		void PrintWaiting();
		void PrintError(const __FlashStringHelper *message);
		void PrintTransmissionInfo(TXResult result);
		void Clear();
		void ButtonA();
		void ButtonB();
		void ButtonC();

	private:
		void PrintRSSI(TXResult *result, uint16_t color);
		void PrintTemperature(SensorData *data, uint16_t color);
		void PrintHumidity(SensorData *data, uint16_t color);
		void PrintPressure(SensorData *data, uint16_t color);
		void PrintParticulates(SensorData *data, uint16_t color);
	};
}
