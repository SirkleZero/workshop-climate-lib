#pragma once

#include <Arduino.h>

namespace RFM69 {
	/// <summary>Represents the status of sending a radio packet.</summary>
	struct TXResult {
		/// <summary>true if the transmission was successful; otherwise false.</summary>
		bool TransmitSuccessful = false;
		/// <summary>The Source of the transmission.</summary>
		uint8_t From = 0;
		/// <summary>The signal strength of the transmission.</summary>
		int16_t RSSI = 0;

		/// <summary>Prints a debug statement to Serial output.</summary>
		void PrintDebug()
		{
			Serial.println(F("---------------------------------------"));
			Serial.print(F("TransmitSuccessful = ")); Serial.println(TransmitSuccessful);
			Serial.print(F("From = ")); Serial.println(From);
			Serial.print(F("RSSI = ")); Serial.println(RSSI);
			Serial.println(F("---------------------------------------"));
			Serial.println();
		}
	};
}
