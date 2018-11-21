#ifndef FeatherOLEDProxy_h
#define FeatherOLEDProxy_h

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "..\Sensors\SensorData.h"
#include "..\TX\TXResult.h"

using namespace Sensors;
using namespace TX;

namespace Display {
	/// <summary>Describes the state of the buttons of the Feather OLED display.</summary>
	enum ButtonMode {
		Default = 0x00,
		A = 0x01,
		B = 0x02,
		C = 0x03
	};

	/// <summary>Defines a class that represents the display of the Sensor module.</summary>
	class FeatherOLEDProxy {
	public:
		FeatherOLEDProxy();

		void Initialize();
		void PrintSensors(SensorData data);
		void PrintWaiting();
		void PrintRFM69Update(TXResult *result);
		void Clear();
		void ButtonA();
		void ButtonB();
		void ButtonC();

	private:
		void PrintTemperature(SensorData *data, uint16_t color);
		void PrintHumidity(SensorData *data, uint16_t color);
		void PrintPressure(SensorData *data, uint16_t color);
		void PrintParticulates(SensorData *data, uint16_t color);
	};
}

#endif
