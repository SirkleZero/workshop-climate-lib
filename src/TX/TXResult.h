#ifndef TXResult_h
#define TXResult_h

#include <Arduino.h>

namespace TX {
	/// <summary>Represents the status of sending a radio packet.</summary>
	struct TXResult {
		/// <summary>true if the transmission was successful; otherwise false.</summary>
		bool TransmitSuccessful = false;
		/// <summary>The Source of the transmission.</summary>
		uint8_t From;
		/// <summary>The signal strength of the transmission.</summary>
		int16_t RSSI;

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
#endif
