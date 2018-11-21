#ifndef IoTUploadResult_h
#define IoTUploadResult_h

#include <Arduino.h>
#include <WiFi101.h>

namespace TX {
	/// <summary>Represents the status of uploading data to the IoT provider.</summary>
	struct IoTUploadResult {
		/// <summary>true if the upload was successful; otherwise false.</summary>
		bool IsSuccess = false;
		/// <summary>The SSID of the WiFi connection we are connected to.</summary>
		char* SSID;
		/// <summary>The signal strength of the WiFi connection.</summary>
		int16_t RSSI;
		/// <summary>The local area network IP address assigned to the microcontroller.</summary>
		IPAddress LocalIP;
		/// <summary>The gateway IP address of the network the microcontroller is connected to.</summary>
		IPAddress GatewayIP;
		/// <summary>The subnet mask of the network the microcontroller is connected to.</summary>
		IPAddress SubnetMask;
		/// <summary>The flash string helper containing error messages if they are present.</summary>
		const __FlashStringHelper *ErrorMessage;

		/// <summary>Prints a debug statement to Serial output.</summary>
		void PrintDebug()
		{
			Serial.println(F("---------------------------------------"));
			Serial.print(F("Is Success: ")); Serial.println(this->IsSuccess);
			Serial.print(F("SSID: ")); Serial.println(this->SSID);
			Serial.print(F("RSSI: ")); Serial.println(this->RSSI);
			Serial.print(F("Local IP: ")); Serial.println(this->LocalIP);
			Serial.print(F("Gateway IP: ")); Serial.println(this->GatewayIP);
			Serial.print(F("Subnet Mask: ")); Serial.println(this->SubnetMask);
			Serial.print(F("Error Message: ")); Serial.println(this->ErrorMessage);
			Serial.println(F("---------------------------------------"));
			Serial.println();
		}
	};
}
#endif
