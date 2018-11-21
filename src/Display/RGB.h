#ifndef RGB_h
#define RGB_h

#include <Arduino.h>

namespace Display {
	/// <summary>Defines a structure that represents an RGB value.</summary>
	struct RGB {
		/// <summary>Initializes a new instance of the <see cref="RGB"/> class.</summary>
		/// <param name="red">The integer value of red.</param>
		/// <param name="green">The integer value of green.</param>
		/// <param name="blue">The integer value of blue.</param>
		RGB(int red, int green, int blue) : Red(red), Green(green), Blue(blue) {}

		/// <summary>The integer value of red.</summary>
		int Red = 0;
		/// <summary>The integer value of green.</summary>
		int Green = 0;
		/// <summary>The integer value of blue.</summary>
		int Blue = 0;

		/// <summary>Prints a debug statement to Serial output.</summary>
		virtual void PrintDebug()
		{
			Serial.println(F("---------------------------------------"));
			Serial.print(F("Red: ")); Serial.println(this->Red);

			Serial.print(F("Green: ")); Serial.println(this->Green);

			Serial.print(F("Blue: ")); Serial.println(this->Blue);
			Serial.println(F("---------------------------------------"));
			Serial.println();
		}
	};
}
#endif
