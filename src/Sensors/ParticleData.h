#pragma once

#include <Arduino.h>

namespace Sensors {
	/// <summary>Represents data that describes airborn particulate matter.</summary>
	struct ParticleData {
		/// <summary>Indoor air quality indicator of particles 1.0 micron in size.</summary>
		uint16_t pm10_standard;
		/// <summary>Indoor air quality indicator of particles 2.5 micron in size.</summary>
		uint16_t pm25_standard;
		/// <summary>Indoor air quality indicator of particles 10.0 micron in size.</summary>
		uint16_t pm100_standard;
		/// <summary>Environmental air quality indicator of particles 1.0 micron in size.</summary>
		uint16_t pm10_env;
		/// <summary>Environmental air quality indicator of particles 2.5 micron in size.</summary>
		uint16_t pm25_env;
		/// <summary>Environmental air quality indicator of particles 10.0 micron in size.</summary>
		uint16_t pm100_env;
		/// <summary>Measurement of .03 micron sized particles per cubic meter of air.</summary>
		uint16_t particles_03um;
		/// <summary>Measurement of .05 micron sized particles per cubic meter of air.</summary>
		uint16_t particles_05um;
		/// <summary>Measurement of 1.0 micron sized particles per cubic meter of air.</summary>
		uint16_t particles_10um;
		/// <summary>Measurement of 2.5 micron sized particles per cubic meter of air.</summary>
		uint16_t particles_25um;
		/// <summary>Measurement of 5.0 micron sized particles per cubic meter of air.</summary>
		uint16_t particles_50um;
		/// <summary>Measurement of 10.0 micron sized particles per cubic meter of air.</summary>
		uint16_t particles_100um;

		/// <summary>Prints a debug statement to Serial output.</summary>
		void PrintDebug()
		{
			Serial.println(F("---------------------------------------"));
			Serial.println(F("Concentration Units (standard)"));
			Serial.print(F("PM 1.0: ")); Serial.print(pm10_standard);
			Serial.print(F("\t\tPM 2.5: ")); Serial.print(pm25_standard);
			Serial.print(F("\t\tPM 10: ")); Serial.println(pm100_standard);
			Serial.println(F("---------------------------------------"));
			Serial.println(F("Concentration Units (environmental)"));
			Serial.print(F("PM 1.0: ")); Serial.print(pm10_env);
			Serial.print(F("\t\tPM 2.5: ")); Serial.print(pm25_env);
			Serial.print(F("\t\tPM 10: ")); Serial.println(pm100_env);
			Serial.println(F("---------------------------------------"));
			Serial.print(F("Particles > 0.3um / 0.1L air:")); Serial.println(particles_03um);
			Serial.print(F("Particles > 0.5um / 0.1L air:")); Serial.println(particles_05um);
			Serial.print(F("Particles > 1.0um / 0.1L air:")); Serial.println(particles_10um);
			Serial.print(F("Particles > 2.5um / 0.1L air:")); Serial.println(particles_25um);
			Serial.print(F("Particles > 5.0um / 0.1L air:")); Serial.println(particles_50um);
			Serial.print(F("Particles > 10.0 um / 0.1L air:")); Serial.println(particles_100um);
			Serial.println(F("---------------------------------------"));
			Serial.println();
		}
	};
}
