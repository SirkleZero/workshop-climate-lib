#ifndef RGB_h
#define RGB_h

#include <Arduino.h>

namespace Display {
	/// <summary>Defines a structure that represents an RGB value.</summary>
	struct RGB565 {
		/// <summary>Initializes a new instance of the <see cref="RGB"/> class.</summary>
		/// <param name="red">The integer value of red.</param>
		/// <param name="green">The integer value of green.</param>
		/// <param name="blue">The integer value of blue.</param>
		RGB565(uint8_t red, uint8_t green, uint8_t blue) : Red(red), Green(green), Blue(blue) {}

		/// <summary>The integer value of red.</summary>
		uint8_t Red = 0;
		/// <summary>The integer value of green.</summary>
		uint8_t Green = 0;
		/// <summary>The integer value of blue.</summary>
		uint8_t Blue = 0;

		/// <summary>Converts the instance of this object to a decimal equivalent following the RGB565 spec.</summary>
		uint16_t ToDecimal()
		{
			return RGB565::ToDecimal(this->Red, this->Green, this->Blue);
		}

		/// <summary>Converts RGB values to a decimal equivalent following the RGB565 spec.</summary>
		/// <param name="red">The integer value of red.</param>
		/// <param name="green">The integer value of green.</param>
		/// <param name="blue">The integer value of blue.</param>
		static uint16_t ToDecimal(uint8_t red, uint8_t green, uint8_t blue)
		{
			// http://www.barth-dev.de/online/rgb565-color-picker/
			uint16_t Rgb565 = 0;
			Rgb565 = (((red & 0b11111000) << 8) + ((green & 0b11111100) << 3) + (blue >> 3));
			return Rgb565;
		}

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
