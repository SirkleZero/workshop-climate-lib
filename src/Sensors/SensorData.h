#ifndef SensorData_h
#define SensorData_h

#include <Arduino.h>

#include "ClimateData.h"
#include "ParticleData.h"

namespace Sensors {
	/// <summary>Represents data that is collected by the sensor module.</summary>
	struct SensorData {
		/// <summary>Data that represents climate information.</summary>
		ClimateData Climate;
		/// <summary>Data that represents airborn particulates.</summary>
		ParticleData Particulates;

		/// <summary>Prints a debug statement to Serial output.</summary>
		void PrintDebug()
		{
			Climate.PrintDebug();
			Particulates.PrintDebug();
		}

		/// <summary>Factory method to create a <see cref="SensorData"/> that contains no data.</summary>
		static SensorData EmptyData()
		{
			SensorData sensorData;
			sensorData.Climate.Temperature = 0;
			sensorData.Climate.Humidity = 0;
			sensorData.Climate.Pressure = 0;

			sensorData.Particulates.pm10_standard = 0;
			sensorData.Particulates.pm25_standard = 0;
			sensorData.Particulates.pm100_standard = 0;
			sensorData.Particulates.pm10_env = 0;
			sensorData.Particulates.pm25_env = 0;
			sensorData.Particulates.pm100_env = 0;
			sensorData.Particulates.particles_03um = 0;
			sensorData.Particulates.particles_05um = 0;
			sensorData.Particulates.particles_10um = 0;
			sensorData.Particulates.particles_25um = 0;
			sensorData.Particulates.particles_50um = 0;
			sensorData.Particulates.particles_100um = 0;

			return sensorData;
		}
	};
}
#endif
