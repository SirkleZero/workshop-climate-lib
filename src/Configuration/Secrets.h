#ifndef Secrets_h
#define Secrets_h

#include <Arduino.h>

namespace Configuration {
	/// <summary>Class that represents secrets used by the system.</summary>
	class Secrets {
	public:
		/// <summary>The encryption key used by the application to encrypt radio transmission.</summary>
		static uint8_t RadioEncryptionKey[16];
		/// <summary>The Adafruit IO username.</summary>
		const char *AdafruitIOUsername;
		/// <summary>The access key for Adafruit IO service calls.</summary>
		const char *AdafruitIOAccessKey;
		/// <summary>The WiFi network SSID to connect to for internet access.</summary>
		const char *WiFiSSID;
		/// <summary>The password for the WiFi network.</summary>
		const char *WiFiPassword;
		/// <summary>The network hostname to assign to the device.</summary>
		const char *Hostname;

		Secrets();

		void SetAdafruitIOUsername(const char * adafruitIOUsername);
		void SetAdafruitIOAccessKey(const char * adafruitIOAccessKey);
		void SetWifiSSID(const char * wifiSSID);
		void SetWifiPassword(const char * wifiPassword);
		void SetHostname(const char * hostname);
		void PrintDebug();
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
		String hostname;
	};
}
#endif
