#pragma once

#include <Arduino.h>

///<summary>Defines the structure of a rectangle.</summary>
struct Rectangle
{
	/// <summary>
	///	Creates an instance of the Rectangle struct.
	/// </summary>
	/// <param name = "x">The x coordinate of the rectangle.</param>
	/// <param name = "y">The y coordinate of the rectangle.</param>
	/// <param name = "width">The width of the rectangle.</param>
	/// <param name = "height">The height of the rectangle.</param>
	Rectangle(int16_t x = 0, int16_t y = 0, int16_t width = 0, int16_t height = 0) :
		x(x), y(y), width(width), height(height) {}

	///<summary>The x coordinate of the rectangle.</summary>
	int16_t x = 0;
	///<summary>The y coordinate of the rectangle.</summary>
	int16_t y = 0;
	///<summary>The width of the rectangle.</summary>
	int16_t width = 0;
	///<summary>The height of the rectangle.</summary>
	int16_t height = 0;

	///<summary>Prints a debug statement to Serial output.</summary>
	virtual void PrintDebug()
	{
		Serial.println(F("---------------------------------------"));
		Serial.print(F("x: ")); Serial.println(x);
		Serial.print(F("y: ")); Serial.println(y);
		Serial.print(F("height: ")); Serial.println(height);
		Serial.print(F("width: ")); Serial.println(width);
		Serial.println(F("---------------------------------------"));
		Serial.println();
	}
};
