#include "SensorManager.h"

namespace Sensors {
	/// <summary>Initializes a new instance of the <see cref="SensorManager"> class.</summary>
	/// <param name="enabledSensors">Specifies which sensors should be enabled.</param>
	/// <param name="temperatureUnit">Specifies the temperature unit that will be used by the temperature sensor.</param>
	/// <param name="updateInterval">Specifies how often in milliseconds the timer interrupt will trigger.</param>
	SensorManager::SensorManager(AvailableSensors enabledSensors, TemperatureUnit temperatureUnit, unsigned long updateInterval) :
		climateProxy(temperatureUnit)
	{
		this->enabledSensors = enabledSensors;
		this->temperatureUnit = temperatureUnit;
		this->updateInterval = updateInterval;
	}

	/// <summary>Executes initialization logic for the object.</summary>
	/// <returns>An <see cref="InitializationResult"/> that describes the result of initialization.</returns>
	InitializationResult SensorManager::Initialize()
	{
		InitializationResult result;
		Serial.print("enabledsensors: "); Serial.println(this->enabledSensors);

		// check to see if the BME 280 is enabled to run.
		if ((this->enabledSensors & AvailableSensors::BME280) == AvailableSensors::BME280)
		{
			Serial.println("Loading the bme280");
			// initialize the BME280
			this->bmeInitializationResult = this->climateProxy.Initialize();
			if (!bmeInitializationResult.IsSuccessful)
			{
				result.ErrorMessage = F("Failed to load the BME280 Sensor.");
				return result;
			}
		}
		// checks to see if the PMS5003 has been specified to run
		if ((this->enabledSensors & AvailableSensors::PMS5003) == AvailableSensors::PMS5003)
		{
			Serial.println("Loading the pms5003");
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

	/// <summary>Reads information from the enabled sensors.</summary>
	/// <param name="data">The <see cref="SensorData"> that will contain sensor information after the method completes execution.</param>
	bool SensorManager::ReadSensors(SensorData *data)
	{
		bool returnValue = false;
		this->currentMillis = millis();

		// simple timer implementation.
		if (((this->currentMillis - this->lastUpdate) > this->updateInterval) || this->isFirstIteration)
		{
			this->lastUpdate = millis();
			this->isFirstIteration = false;
			bool sensorsSucceeded = true;

			// for each enabled sensor, read its information.
			if ((this->enabledSensors & AvailableSensors::BME280) == AvailableSensors::BME280 && this->bmeInitializationResult.IsSuccessful)
			{
				sensorsSucceeded &= this->climateProxy.ReadSensor(data);
			}
			if ((this->enabledSensors & AvailableSensors::PMS5003) == AvailableSensors::PMS5003 && this->pmsInitializationResult.IsSuccessful)
			{
				sensorsSucceeded &= this->particleProxy.ReadSensor(data);
			}

			returnValue = sensorsSucceeded;
		}

		return returnValue;
	}

	SensorManager::~SensorManager() {}
}
