#pragma once

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "..\InitializationResult.h"
#include "BME280Data.h"

namespace Sensors {
	/// <summary>A class that handles the communication with the BME280 sensor.</summary>
	class BME280Proxy {
	public:
		/// <summary>A value that represents air pressure at sea level. NOTE: this is not a constant, but an accepted value in lieu of one.</summary>
		static const float SeaLevelPressure_hPa;

		BME280Proxy();

		InitializationResult Initialize(TemperatureUnit units = TemperatureUnit::C, unsigned long updateInterval = BME280Proxy::DefaultUpdateInterval);
		bool ReadSensor(BME280Data *data);
		void SetUnit(TemperatureUnit units = TemperatureUnit::C);
		void PrintDebug();

	private:
		static const unsigned long DefaultUpdateInterval = 60000;
		Adafruit_BME280 bme;
		TemperatureUnit units;

		// timer - used for controlling when we take sensor readings and transmit them
		unsigned long currentMillis = 0; // stores the current value of millis()
		unsigned long lastUpdate = 0; // stores the last time we tried to execute a sensor reading
		unsigned long updateInterval = BME280Proxy::DefaultUpdateInterval;
		bool isFirstIteration = true;
	};
}
