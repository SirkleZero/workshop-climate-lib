#ifndef Secrets_h
#define Secrets_h

#include <Arduino.h>

namespace Configuration {
	/// <summary>Class that represents secrets used by the system.</summary>
	class Secrets {
	private:
		/*
		HACK Incomming!!!

		These private variables hold the values for the secrets used by the system. This is done this way to handle the fact
		that these values come to us as const char * from the SD proxy underlying system. Once stuffed into the String type
		these are then converted back to const char * for actual use by the various parts of our system.

		Yes, this is lame, but I'm not sure of a better way to handle copying const char * values into a global object when they 
		aren't in fact constants, but are set via a config file that goes out of scope once the method that populates the data
		is complete.
		*/
		String adafruitIOUsername;
		String adafruitIOAccessKey;
		String wiFiSSID;
		String wiFiPassword;

	public:
		/// <summary>The Adafruit IO username.</summary>
		const char *AdafruitIOUsername;
		/// <summary>The access key for Adafruit IO service calls.</summary>
		const char *AdafruitIOAccessKey;
		/// <summary>The WiFi network SSID to connect to for internet access.</summary>
		const char *WiFiSSID;
		/// <summary>The password for the WiFi network.</summary>
		const char *WiFiPassword;

		/// <summary>Sets the Adafruit IO username.</summary>
		/// <param name="adafruitUsername">The Adafruit IO username.</param>
		void SetAdafruitIOUsername(const char * adafruitIOUsername)
		{
			this->adafruitIOUsername = String(adafruitIOUsername);
			this->AdafruitIOUsername = this->adafruitIOUsername.c_str();
		}

		/// <summary>Sets the key for Adafruit IO service calls.</summary>
		/// <param name="adafruitPassword">The access key for Adafruit IO service calls.</param>
		void SetAdafruitIOAccessKey(const char * adafruitIOAccessKey)
		{
			this->adafruitIOAccessKey = String(adafruitIOAccessKey);
			this->AdafruitIOAccessKey = this->adafruitIOAccessKey.c_str();
		}

		/// <summary>Sets the WiFi network SSID to connect to for internet access.</summary>
		/// <param name="wifiSSID">The WiFi network SSID to connect to for internet access.</param>
		void SetWifiSSID(const char * wifiSSID)
		{
			this->wiFiSSID = String(wifiSSID);
			this->WiFiSSID = this->wiFiSSID.c_str();
		}

		/// <summary>Sets the password for the WiFi network.</summary>
		/// <param name="wifiPassword">The password for the WiFi network.</param>
		void SetWifiPassword(const char * wifiPassword)
		{
			this->wiFiPassword = String(wifiPassword);
			this->WiFiPassword = this->wiFiPassword.c_str();
		}

		///<summary>Prints a debug statement to Serial output.</summary>
		void PrintDebug()
		{
			Serial.println(F("---------------------------------------"));
			Serial.print(F("adafruitio_username = ")); Serial.println(this->AdafruitIOUsername);

			Serial.print(F("adafruitio_key = ")); Serial.println(this->AdafruitIOAccessKey);

			Serial.print(F("wifi_ssid = ")); Serial.println(this->WiFiSSID);

			Serial.print(F("wifi_password = ")); Serial.println(this->WiFiPassword);
			Serial.println(F("---------------------------------------"));
			Serial.println();
		}
	};
}
#endif
