#pragma once

// external dependencies
#include <Arduino.h>
#include <Adafruit_HX8357.h>

#include "TFTDisplay.h"

using namespace Sensors;

namespace Display {
	/// <summary>Defines a class that controls the display of the monitor unit.</summary>
	class TFT35 : public TFTDisplay
	{
	public:
		TFT35();
	private:
	};
}
