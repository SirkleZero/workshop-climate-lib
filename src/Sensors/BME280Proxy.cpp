#include "BME280Proxy.h"

namespace Sensors {
	namespace BME280 {
		static const float SeaLevelPressure_hPa = 1013.25;

		/// <summary>Initializes a new instance of the <see cref="BME280Proxy"/> class.</summary>
		BME280Proxy::BME280Proxy(TemperatureUnit units)
		{
			this->units = units;
		}

		/// <summary>Executes initialization logic for the object.</summary>
		/// <returns>An <see cref="InitializationResult"/> that describes the result of initialization.</returns>
		InitializationResult BME280Proxy::Initialize()
		{
			InitializationResult result;

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

		/// <summary>Reads data from the sensor.</summary>
		/// <param name="data">The <see cref="SensorData"/> object to place the data into.</param>
		bool BME280Proxy::ReadSensor(SensorData *data)
		{
			// we are taking a forced measurement based on the setSampling settings we've specified. If those change, then this needs to be revisited according to the spec of the sensor.
			bme.takeForcedMeasurement();

			switch (this->units)
			{
				case C:
					data->Climate.Temperature = bme.readTemperature();
					data->Climate.Units = 'C';
					break;
				case F:
					data->Climate.Temperature = ClimateData::ConvertCToF(bme.readTemperature());
					data->Climate.Units = 'F';
					break;
				default:
					data->Climate.Temperature = bme.readTemperature();
					data->Climate.Units = 'C';
					break;
			}

			data->Climate.Pressure = bme.readPressure();
			data->Climate.Humidity = bme.readHumidity();

			return true;
		}

		/// <summary>Prints a debug statement to Serial output.</summary>
		void BME280Proxy::PrintDebug()
		{
			SensorData data;
			this->ReadSensor(&data);

			Serial.println(F("---------------------------------------"));
			Serial.print(F("Temperature = ")); Serial.print(data.Climate.Temperature); Serial.print(F(" *")); Serial.println(this->units);
			Serial.print(F("Pressure = ")); Serial.print(data.Climate.Pressure / 100.0F); Serial.println(F(" hPa"));
			Serial.print(F("Humidity = ")); Serial.print(data.Climate.Humidity); Serial.println(F(" %"));
			Serial.println(F("---------------------------------------"));
			Serial.println();
		}
	}
}
