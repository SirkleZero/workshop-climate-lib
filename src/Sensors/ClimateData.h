#ifndef ClimateData_h
#define ClimateData_h

#include <Arduino.h>
#include <avr/dtostrf.h>

namespace Sensors {
	struct ClimateData {
		float Temperature;   // [deg C]
		char Units;
		float Humidity;      // [%]
		float Pressure;      // pascals

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

		static float ConvertCToF(float c)
		{
			return (c * 9 / 5) + 32;
		}

		static char* ConvertFloatToString(float f, signed char width, unsigned char precision)
		{
			char buff[10];
			dtostrf(f, width, precision, buff);
			return buff;
		}
	};
}
#endif
