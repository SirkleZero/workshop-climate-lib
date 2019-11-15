#include "ArrayThing.h"

namespace Sensors {
	/// <summary>Initializes a new instance of the <see cref="ArrayThing"/> class.</summary>
	ArrayThing::ArrayThing() {}

	void ArrayThing::Add(float f)
	{
		this->total = this->total - this->readings[this->readIndex];

		this->readings[this->readIndex] = f;
		

		this->total = this->total + this->readings[this->readIndex];
		

		this->readIndex = this->readIndex +1;

		if (this->readIndex >= ArrayThing::numReadings)
		{
			this->readIndex = 0;
		}
		if (this->addedItems < ArrayThing::numReadings)
		{
			this->addedItems = this->addedItems+1;
		}

		/*Serial.print(F("f: ")); Serial.println(f);
		Serial.print(F("this->total: ")); Serial.println(this->total);
		Serial.print(F("this->readIndex: ")); Serial.println(this->readIndex);
		Serial.print(F("ArrayThing::numReadings: ")); Serial.println(ArrayThing::numReadings);
		Serial.print(F("this->addedItems: ")); Serial.println(this->addedItems);*/
		Serial.print(F("Free Memory: ")); Serial.println(freeMemory());
	}

	float ArrayThing::Average()
	{
		return this->total / this->addedItems;
	}
}
