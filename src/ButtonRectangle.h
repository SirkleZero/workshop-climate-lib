#ifndef ButtonRectangle_h
#define ButtonRectangle_h

#include <Arduino.h>
#include "Rectangle.h"

///<summary>Defines the structure of a rectangle.</summary>
struct ButtonRectangle : Rectangle
{
	/// <summary>
	///	Creates an instance of the Rectangle struct.
	/// </summary>
	/// <param name = "x">The x coordinate of the rectangle.</param>
	/// <param name = "y">The y coordinate of the rectangle.</param>
	/// <param name = "width">The width of the rectangle.</param>
	/// <param name = "height">The height of the rectangle.</param>
	ButtonRectangle(int16_t x, int16_t y, int16_t width, int16_t height) :
		Rectangle(x, y, width, height)
	{}

	void Disable()
	{
		this->enabled = false;
	}

	bool IsEnabled()
	{
		return this->enabled;
	}

	void Enable()
	{
		this->enabled = true;
	}

	/// <summary>
	///	Calculates if a point is contained by the rectangle, inclusive of its borders.
	/// </summary>
	/// <param name = "x">The x coordinate of the point.</param>
	/// <param name = "y">The y coordinate of the point.</param>
	bool Contains(int16_t x, int16_t y)
	{
		return
			x >= this->x && x <= this->x + this->width &&
			y >= this->y && y <= this->y + this->height;
	}

	///<summary>Prints a debug statement to Serial output.</summary>
	virtual void PrintDebug()
	{
		Serial.println(F("---------------------------------------"));
		Serial.print(F("x: ")); Serial.println(x);
		Serial.print(F("y: ")); Serial.println(y);
		Serial.print(F("height: ")); Serial.println(height);
		Serial.print(F("width: ")); Serial.println(width);
		Serial.print(F("enabled: ")); Serial.println(enabled);
		Serial.println(F("---------------------------------------"));
		Serial.println();
	}

private:
	bool enabled = true;
};
#endif
