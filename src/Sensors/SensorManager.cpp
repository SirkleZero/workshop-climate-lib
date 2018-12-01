#include "SensorManager.h"

namespace Sensors {
	SensorManager::SensorManager(AvailableSensors enabledSensors, TemperatureUnit temperatureUnit) : 
		climateProxy(temperatureUnit)
	{
		this->enabledSensors = enabledSensors;
		this->temperatureUnit = temperatureUnit;
	}

	InitializationResult SensorManager::Initialize()
	{
		InitializationResult result;

		if (this->enabledSensors & AvailableSensors::BME280)
		{
			// initialize the BME280
			this->bmeInitializationResult = this->climateProxy.Initialize();
			if (!bmeInitializationResult.IsSuccessful)
			{
				result.ErrorMessage = F("Failed to load the BME280 Sensor.");
				return result;
			}

		}
		else if (this->enabledSensors & AvailableSensors::PMS5003)
		{
			// initialize the PMS5003
			this->pmsInitializationResult = this->particleProxy.Initialize();
			if (!pmsInitializationResult.IsSuccessful)
			{
				result.ErrorMessage = F("Failed to load the PMS5003 Sensor.");
				return result;
			}
		}

		result.IsSuccessful = true;
		return result;
	}

	bool SensorManager::ReadSensors(SensorData *data)
	{
		bool returnValue = true;

		if (this->enabledSensors & AvailableSensors::BME280 && this->bmeInitializationResult.IsSuccessful)
		{
			returnValue &= this->climateProxy.ReadSensor(data);
		}
		else if (this->enabledSensors & AvailableSensors::PMS5003 && this->pmsInitializationResult.IsSuccessful)
		{
			returnValue &= this->particleProxy.ReadSensor(data);
		}

		return returnValue;
	}

	SensorManager::~SensorManager() {}
}
