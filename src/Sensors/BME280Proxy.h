#ifndef BME280Proxy_h
#define BME280Proxy_h

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "..\InitializationResult.h"
#include "SensorData.h"

namespace Sensors {
	/// <summary>Specifies values that represent the various temperature units supported.</summary>
	enum TemperatureUnit {
		/// <summary>The temperature is represented by degrees celsius.</summary>
		C = 0x00,
		/// <summary>The temperature is represented by degrees fahrenheit.</summary>
		F = 0x01
	};

	/// <summary>A class that handles the communication with the BME280 sensor.</summary>
	class BME280Proxy {
	public:
		/// <summary>A value that represents air pressure at sea level. NOTE: this is not a constant, but an accepted value in lieu of one.</summary>
		static const float SeaLevelPressure_hPa;

		BME280Proxy(TemperatureUnit units = TemperatureUnit::C);

		InitializationResult Initialize();
		bool ReadSensor(SensorData *data);
		void PrintDebug();

	private:
		Adafruit_BME280 bme;
		TemperatureUnit units;
	};
}
#endif
