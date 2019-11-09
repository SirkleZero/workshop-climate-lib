#include "BME280Proxy.h"

namespace Sensors {
	static const float SeaLevelPressure_hPa = 1013.25;

	/// <summary>Initializes a new instance of the <see cref="BME280Proxy"/> class.</summary>
	BME280Proxy::BME280Proxy() {}

	/// <summary>Executes initialization logic for the object.</summary>
	/// <returns>An <see cref="InitializationResult"/> that describes the result of initialization.</returns>
	InitializationResult BME280Proxy::Initialize(TemperatureUnit units, unsigned long updateInterval)
	{
		InitializationResult result;
		this->units = units;
		this->updateInterval = updateInterval;

		// initialize the BME280 sensor          
		if (!bme.begin(&Wire))
		{
			result.ErrorMessage = F("Could not find a valid BME280 sensor, check wiring!");
			return result;
		}

		// configure the BME280 sensor, // suggested rate is 1/60Hz (1s)
		bme.setSampling(Adafruit_BME280::MODE_FORCED,
			Adafruit_BME280::SAMPLING_X1,   // temperature
			Adafruit_BME280::SAMPLING_X1, // pressure
			Adafruit_BME280::SAMPLING_X1,   // humidity
			Adafruit_BME280::FILTER_OFF);

		result.IsSuccessful = true;
		return result;
	}

	void BME280Proxy::SetUnit(TemperatureUnit units)
	{
		this->units = units;
	}

	/// <summary>Reads data from the sensor.</summary>
	/// <param name="data">The <see cref="BME280Data"/> object to place the data into.</param>
	bool BME280Proxy::ReadSensor(BME280Data *data)
	{
		bool returnValue = false;
		this->currentMillis = millis();

		// simple timer implementation.
		if (((this->currentMillis - this->lastUpdate) > this->updateInterval) || this->isFirstIteration)
		{
			this->lastUpdate = millis();
			this->isFirstIteration = false;

			// we are taking a forced measurement based on the setSampling settings we've 
			// specified. If those change, then this needs to be revisited according to 
			// the spec of the sensor.
			bme.takeForcedMeasurement();

			switch (this->units)
			{
				case C:
					data->Temperature = bme.readTemperature();
					data->Units = 'C';
					break;
				case F:
					data->Temperature = BME280Data::ConvertCToF(bme.readTemperature());
					data->Units = 'F';
					break;
				default:
					data->Temperature = bme.readTemperature();
					data->Units = 'C';
					break;
			}

			data->Pressure = bme.readPressure();
			data->Humidity = bme.readHumidity();

			returnValue = true;
		}

		return returnValue;
	}

	/// <summary>Prints a debug statement to Serial output.</summary>
	void BME280Proxy::PrintDebug()
	{
		BME280Data data;
		this->ReadSensor(&data);

		Serial.println(F("---------------------------------------"));
		Serial.print(F("Temperature = ")); Serial.print(data.Temperature); Serial.print(F(" *")); Serial.println(this->units);
		Serial.print(F("Pressure = ")); Serial.print(data.Pressure / 100.0F); Serial.println(F(" hPa"));
		Serial.print(F("Humidity = ")); Serial.print(data.Humidity); Serial.println(F(" %"));
		Serial.println(F("---------------------------------------"));
		Serial.println();
	}
}
