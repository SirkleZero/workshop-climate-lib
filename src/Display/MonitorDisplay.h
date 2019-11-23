#ifndef MonitorDisplay_h
#define MonitorDisplay_h


// external dependencies
#include <Arduino.h>
#include "TFTDisplay.h"

using namespace Sensors;

namespace Display {
	/// <summary>Defines a class that controls the display of the monitor unit.</summary>
	class MonitorDisplay : public TFTDisplay
	{
	public:
		MonitorDisplay();
	private:
	};
}

#endif
