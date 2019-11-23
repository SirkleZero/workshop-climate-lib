#pragma once

#include <Arduino.h>
#include "Sensors\BME280Data.h"

using namespace Sensors;

namespace RX {
	///<summary>Represents the results of sensor information being recieved after having been transmitted.</summary>
	struct SensorTransmissionResult {
		///<summary>The sensor data.</summary>
		BME280Data Data;
		///<summary>A value that indicates if the transmission was successful.</summary>
		bool HasResult = false;
		///<summary>A value that indicates the source of the transmission.</summary>
		uint8_t TransmissionSource;
		///<summary>A value that indicates the relative received signal strength of the transmission.</summary>
		int16_t RSSI;

		///<summary>Prints a debug statement to Serial output.</summary>
		void PrintDebug()
		{
			Serial.println(F("---------------------------------------"));
			Serial.print(F("Got packet from #")); Serial.print(this->TransmissionSource);
			Serial.print(F(" [RSSI :")); Serial.print(this->RSSI); Serial.println(F("]"));
			Serial.println(F("---------------------------------------"));
			Serial.println();

			Data.PrintDebug();
		}
	};
}
