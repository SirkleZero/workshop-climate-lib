#ifndef SensorData_h
#define SensorData_h

#include <Arduino.h>

#include "ClimateData.h"
#include "ParticleData.h"

namespace Sensors {
	/// <summary>Represents data that is collected by the sensor module.</summary>
	struct SensorData {
		/// <summary>Data that represents climate information.</summary>
		ClimateData climate;
		/// <summary>Data that represents airborn particulates.</summary>
		ParticleData particulates;

		/// <summary>Prints a debug statement to Serial output.</summary>
		void PrintDebug()
		{
			climate.PrintDebug();
			particulates.PrintDebug();
		}

		/// <summary>Factory method to create a <see cref="SensorData"/> that contains no data.</summary>
		static SensorData EmptyData()
		{
			SensorData sensorData;
			sensorData.climate.Temperature = 0;
			sensorData.climate.Humidity = 0;
			sensorData.climate.Pressure = 0;

			sensorData.particulates.pm10_standard = 0;
			sensorData.particulates.pm25_standard = 0;
			sensorData.particulates.pm100_standard = 0;
			sensorData.particulates.pm10_env = 0;
			sensorData.particulates.pm25_env = 0;
			sensorData.particulates.pm100_env = 0;
			sensorData.particulates.particles_03um = 0;
			sensorData.particulates.particles_05um = 0;
			sensorData.particulates.particles_10um = 0;
			sensorData.particulates.particles_25um = 0;
			sensorData.particulates.particles_50um = 0;
			sensorData.particulates.particles_100um = 0;

			return sensorData;
		}
	};
}
#endif
