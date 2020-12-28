#pragma once

#include <Arduino.h>
#include <avr/dtostrf.h>

namespace Sensors {
	/// <summary>Specifies values that represent the various temperature units supported.</summary>
	enum TemperatureUnit
	{
		/// <summary>The temperature is represented by degrees celsius.</summary>
		C = 1u << 1,
		/// <summary>The temperature is represented by degrees fahrenheit.</summary>
		F = 1u << 2
	};

	/// <summary>Represents data related to the climate.</summary>
	struct BME280Data {
		/// <summary>The temperature of the environment.</summary>
		float Temperature = 0;
		/// <summary>The units that the temperature is represented in.</summary>
		char Units;
		/// <summary>The relative humidity as a percentage.</summary>
		float Humidity = 0;
		/// <summary>The air pressure in pascals.</summary>
		float Pressure = 0;

		/// <summary>Factory method to create a <see cref="BME280Data"/> that contains no data.</summary>
		static BME280Data EmptyData()
		{
			BME280Data data;
			data.Temperature = 0;
			data.Humidity = 0;
			data.Pressure = 0;

			return data;
		}

		/// <summary>Prints a debug statement to Serial output.</summary>
		void PrintDebug()
		{
			Serial.println(F("---------------------------------------"));
			Serial.print(F("Humidity: ")); Serial.print(Humidity); Serial.println(F(" %"));
			Serial.print(F("Temperature: ")); Serial.print(Temperature); Serial.print(F(" *")); Serial.println(Units);
			Serial.print(F("Pressure: ")); Serial.print(Pressure / 100.0F); Serial.println(F(" hPa")); // pressure in pascals
			Serial.println(F("---------------------------------------"));
			Serial.println();
		}

		/// <summary>Converts a celsius temperature to Fahrenheit.</summary>
		static float ConvertCToF(float c)
		{
			return (c * 9 / 5) + 32;
		}

		/// <summary>Converts a float value to a string.</summary>
		/// <remarks>this is simplly a wrapper on top of the dtostrf function. I did it this way because there wasn't always a dtostrf method available on the SAMD21 platform, and I had to have a custom method to handle this functionality. Now that this function exists, I'm leaving this function in but using it as a wrapper than as custom code for backward compatibility.</remarks>
		/// <example>
		/// 	static char charBufferOutput[15];
		///		BME280Data::ConvertFloatToString(12.32, 2, 0, charBufferOutput);
		///		Serial.print("Our float, converted to a string: ");
		///		Serial.println(charBufferOutput);
		/// </example>
		static char *ConvertFloatToString(float f, signed char width, unsigned char precision, char *sout)
		{
			dtostrf(f, width, precision, sout);
			return sout;
		}
	};
}
