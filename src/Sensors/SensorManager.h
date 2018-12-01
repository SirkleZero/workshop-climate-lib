#ifndef SensorManager_h
#define SensorManager_h

#include <Arduino.h>

#include "..\InitializationResult.h"
#include "BME280Proxy.h"
#include "PMS5003Proxy.h"
#include "SensorData.h"

namespace Sensors {
	enum AvailableSensors {
		None = 1u << 0,
		BME280 = 1u << 1,
		PMS5003 = 1u << 2,
		All = BME280 | PMS5003
	};

	/// <summary>A class that manages the various sensors used by the system.</summary>
	class SensorManager {
	public:
		SensorManager(AvailableSensors enabledSensors = AvailableSensors::None, TemperatureUnit temperatureUnit = TemperatureUnit::C);
		~SensorManager();

		InitializationResult Initialize();
		bool ReadSensors(SensorData *data);
	private:
		AvailableSensors enabledSensors;
		TemperatureUnit temperatureUnit;

		BME280Proxy climateProxy;
		InitializationResult bmeInitializationResult;
		PMS5003Proxy particleProxy;
		InitializationResult pmsInitializationResult;
	};
}
#endif
