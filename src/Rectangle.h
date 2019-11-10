#ifndef Rectangle_h
#define Rectangle_h

#include <Arduino.h>

///<summary>Defines the structure of a rectangle.</summary>
struct Rectangle {
	/// <summary>
	///	Creates an instance of the Rectangle struct.
	/// </summary>
	/// <param name = "x">The x coordinate of the rectangle.</param>
	/// <param name = "y">The y coordinate of the rectangle.</param>
	/// <param name = "width">The width of the rectangle.</param>
	/// <param name = "height">The height of the rectangle.</param>
	Rectangle(int16_t x, int16_t y, int16_t width, int16_t height) : x(x), y(y), width(width), height(height) {}

	///<summary>The x coordinate of the rectangle.</summary>
	int16_t x;
	///<summary>The y coordinate of the rectangle.</summary>
	int16_t y;
	///<summary>The width of the rectangle.</summary>
	int16_t width;
	///<summary>The height of the rectangle.</summary>
	int16_t height;

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
		Serial.println(F("---------------------------------------"));
		Serial.println();
	}
};

///<summary>Defines the structure of a rectangle that is used to draw an air quality index scale.</summary>
struct AqiScaleRectangle : Rectangle {
	/// <summary>
	///	Creates an instance of the AqiScaleRectangle struct.
	/// </summary>
	/// <param name = "x">The x coordinate of the rectangle.</param>
	/// <param name = "y">The y coordinate of the rectangle.</param>
	/// <param name = "width">The width of the rectangle.</param>
	/// <param name = "height">The height of the rectangle.</param>
	/// <param name = "label">The textual representation of this rectangles air quality index value.</param>
	/// <param name = "color">The color to use when rendering this rectangle.</param>
	/// <param name = "minIndex">The minimum value of the air quality index represented by this rectangle (segment) of the AQI index.</param>
	/// <param name = "maxIndex">The maximum value of the air quality index represented by this rectangle (segment) of the AQI index.</param>
	AqiScaleRectangle(int16_t x, int16_t y, int16_t width, int16_t height, const char *label, uint16_t color, int16_t minIndex, int16_t maxIndex) : Rectangle(x, y, width, height), Label(label), Color(color), MinIndex(minIndex), MaxIndex(maxIndex) {}

	///<summary>The textual representation of this rectangles air quality index value.</summary>
	const char *Label;
	///<summary>The color to use when rendering this rectangle.</summary>
	uint16_t Color;
	///<summary>The minimum value of the air quality index represented by this rectangle (segment) of the AQI index.</summary>
	int16_t MinIndex;
	///<summary>The maximum value of the air quality index represented by this rectangle (segment) of the AQI index.</summary>
	int16_t MaxIndex;

	///<summary>Determines if the rectangle is responsible for handling a specific AQI value based on the min and max values.</summary>
	///<param name="aqiScore">The AQI score to test.</param>
	///<returns>returns true if the rectangle is responsible; otherwise fales.</returns>
	bool IsAqiRectangleFor(int16_t aqiScore)
	{
		return aqiScore >= MinIndex && aqiScore <= MaxIndex;
	}

	///<summary>Calulates the location within the rectangles area where a vertical indeicator can be drawn.</summary>
	///<param name="aqiScore">The AQI score to test.</param>
	///<returns>A numeric value within the width of the rectangle, to scale, where the indicator should be drawn.</returns>
	int16_t GetVerticalIndicatorLocation(int16_t aqiScore)
	{
		float divisor = (MaxIndex - MinIndex + 1) / width;
		return ((aqiScore - MinIndex) / divisor) + x;
	}
};
#endif
