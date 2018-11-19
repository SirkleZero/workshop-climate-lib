#ifndef RGB_h
#define RGB_h

#include <Arduino.h>

namespace Display {
	struct RGB {
		RGB(int red, int green, int blue) : Red(red), Green(green), Blue(blue) {}
		int Red = 0;
		int Green = 0;
		int Blue = 0;

		virtual void PrintDebug()
		{
			Serial.println(F("---------------------------------------"));
			Serial.print(F("Red: ")); Serial.println(Red);

			Serial.print(F("Green: ")); Serial.println(Green);

			Serial.print(F("Blue: ")); Serial.println(Blue);
			Serial.println(F("---------------------------------------"));
			Serial.println();
		}
	};
}
#endif
