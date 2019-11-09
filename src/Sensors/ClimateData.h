#ifndef ClimateData_h
#define ClimateData_h

#include <Arduino.h>
#include <avr/dtostrf.h>

namespace Sensors {
	/// <summary>Represents data related to the climate.</summary>
	struct ClimateData {
		/// <summary>The temperature of the environment.</summary>
		float Temperature;
		/// <summary>The units that the temperature is represented in.</summary>
		char Units;
		/// <summary>The relative humidity as a percentage.</summary>
		float Humidity;
		/// <summary>The air pressure in pascals.</summary>
		float Pressure;

		/// <summary>Prints a debug statement to Serial output.</summary>
		void PrintDebug()
		{
			Serial.println(F("---------------------------------------"));
			Serial.print(F("Temperature = "));
			Serial.print(Temperature);
			Serial.print(F(" *"));
			Serial.println(Units);

			Serial.print(F("Pressure = "));
			Serial.print(Pressure / 100.0F);
			Serial.println(F(" hPa")); // pressure in pascals

			Serial.print(F("Humidity = "));
			Serial.print(Humidity);
			Serial.println(F(" %"));
			Serial.println(F("---------------------------------------"));
			Serial.println();
		}

		/// <summary>Converts a celsius temperature to Fahrenheit.</summary>
		static float ConvertCToF(float c)
		{
			return (c * 9 / 5) + 32;
		}

		/// <summary>Converts a float value to a string.</summary>
		/// <remarks>https://forum.arduino.cc/index.php?topic=368720.0</remarks>
		static char* ConvertFloatToString(float f, signed char width, unsigned char precision)
		{
			char buff[10];
			dtostrf(f, width, precision, buff);
			return buff;
		}
	};
}
#endif
