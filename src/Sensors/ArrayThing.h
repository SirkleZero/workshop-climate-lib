#ifndef ArrayThing_h
#define ArrayThing_h

#include <Arduino.h>
#include <MemoryFree.h>

namespace Sensors {
	class ArrayThing
	{
	public:
		ArrayThing();

		void Add(float f);
		float Average();

	private:
		static const int numReadings = 20;

		float readings[ArrayThing::numReadings];      // the readings from the analog input
		int readIndex = 0;							// the index of the current reading
		float total = 0;								// the running total
		float average = 0;							// the average
		int addedItems = 0;							// how many items have been added
	};
}
#endif
