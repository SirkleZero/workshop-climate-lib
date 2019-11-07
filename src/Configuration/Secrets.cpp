#include "Secrets.h"

namespace Configuration {
	// initialize the encryption key for radio communications
	uint8_t Secrets::RadioEncryptionKey[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };

	/// <summary>Initializes a new instance of the <see cref="SDCardProxy"/> class.</summary>
	Secrets::Secrets() {}

	/// <summary>Sets the Adafruit IO username.</summary>
	/// <param name="adafruitUsername">The Adafruit IO username.</param>
	void Secrets::SetAdafruitIOUsername(const char * adafruitIOUsername)
	{
		this->adafruitIOUsername = String(adafruitIOUsername);
		this->AdafruitIOUsername = this->adafruitIOUsername.c_str();
	}

	/// <summary>Sets the key for Adafruit IO service calls.</summary>
	/// <param name="adafruitPassword">The access key for Adafruit IO service calls.</param>
	void Secrets::SetAdafruitIOAccessKey(const char * adafruitIOAccessKey)
	{
		this->adafruitIOAccessKey = String(adafruitIOAccessKey);
		this->AdafruitIOAccessKey = this->adafruitIOAccessKey.c_str();
	}

	/// <summary>Sets the WiFi network SSID to connect to for internet access.</summary>
	/// <param name="wifiSSID">The WiFi network SSID to connect to for internet access.</param>
	void Secrets::SetWifiSSID(const char * wifiSSID)
	{
		this->wiFiSSID = String(wifiSSID);
		this->WiFiSSID = this->wiFiSSID.c_str();
	}

	/// <summary>Sets the password for the WiFi network.</summary>
	/// <param name="wifiPassword">The password for the WiFi network.</param>
	void Secrets::SetWifiPassword(const char * wifiPassword)
	{
		this->wiFiPassword = String(wifiPassword);
		this->WiFiPassword = this->wiFiPassword.c_str();
	}

	/// <summary>Sets the hostname for the device on the network.</summary>
	/// <param name="SetHostname">The hostname for the device on the network.</param>
	void Secrets::SetHostname(const char * hostname)
	{
		this->hostname = String(hostname);
		this->Hostname = this->hostname.c_str();
	}

	///<summary>Prints a debug statement to Serial output.</summary>
	void Secrets::PrintDebug()
	{
		Serial.println(F("---------------------------------------"));
		Serial.print(F("adafruitio_username = ")); Serial.println(this->AdafruitIOUsername);
		Serial.print(F("adafruitio_key = ")); Serial.println(this->AdafruitIOAccessKey);
		Serial.print(F("wifi_ssid = ")); Serial.println(this->WiFiSSID);
		Serial.print(F("wifi_password = ")); Serial.println(this->WiFiPassword);
		Serial.print(F("hostname = ")); Serial.println(this->Hostname);
		Serial.println(F("---------------------------------------"));
		Serial.println();
	}
}
