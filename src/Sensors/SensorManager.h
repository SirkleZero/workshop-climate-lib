#pragma once

#include <Arduino.h>

#include "..\InitializationResult.h"
#include "BME280Proxy.h"
#include "PMS5003Proxy.h"
#include "SensorData.h"

namespace Sensors {
	/// <summary>Provides options for configuring the sensors that will be enabled on the system.</summary>
	enum AvailableSensors {
		/// <summary>None of the sensors are configured.</summary>
		None = 1u << 0,
		/// <summary>Specifies the BME280 sensor should be enabled.</summary>
		BME280 = 1u << 1,
		/// <summary>Specifies the PMS5003 sensor should be enabled.</summary>
		PMS5003 = 1u << 2,
		/// <summary>Specifies all of the sensors should be enabled.</summary>
		All = BME280 | PMS5003
	};

	/// <summary>A class that manages the various sensors used by the system.</summary>
	class SensorManager {
	public:
		SensorManager(AvailableSensors enabledSensors = AvailableSensors::None, TemperatureUnit temperatureUnit = TemperatureUnit::C, unsigned long interval = 60000);
		~SensorManager();

		InitializationResult Initialize();
		InitializationResult Initialize(AvailableSensors enabledSensors, TemperatureUnit temperatureUnit, unsigned long updateInterval);
		bool ReadSensors(SensorData *data);
	private:
		AvailableSensors enabledSensors;
		TemperatureUnit temperatureUnit = TemperatureUnit::C;

		// timer - used for controlling when we take sensor readings and transmit them
		unsigned long currentMillis = 0; // stores the current value of millis()
		unsigned long lastUpdate = 0; // stores the last time we tried to execute a sensor reading
		unsigned long updateInterval = 0;
		bool isFirstIteration = true;

		BME280Proxy bme280Proxy;
		InitializationResult bmeInitializationResult;
		PMS5003Proxy pms5003Proxy;
		InitializationResult pmsInitializationResult;
	};
}
