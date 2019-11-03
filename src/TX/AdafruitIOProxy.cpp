#include "AdafruitIOProxy.h"

using namespace Sensors;
using namespace Configuration;

namespace TX {
	/// <summary>Initializes a new instance of the <see cref="BME280Proxy"/> class.</summary>
	AdafruitIOProxy::AdafruitIOProxy() {}

	/// <summary>Destroys an existing instance of the <see cref="BME280Proxy"/> class.</summary>
	AdafruitIOProxy::~AdafruitIOProxy()
	{
		//delete io;
		//delete mqtt;
	}

	/// <summary>Executes initialization logic for the object.</summary>
	/// <param name="secrets">The <see cref="Secrets"/> that contain information for connecting to the WiFi network and Adafruit IO services.</param>
	/// <returns>An <see cref="InitializationResult"/> that describes the result of initialization.</returns>
	InitializationResult AdafruitIOProxy::Initialize(Secrets *secrets)
	{
		this->secrets = secrets;

		// NOTE: this really was the only way I could actually get this to work. Make sure to delete the io object in the destructor!
		//io = new AdafruitIO_WINC1500(this->secrets->AdafruitIOUsername, this->secrets->AdafruitIOAccessKey, this->secrets->WiFiSSID, this->secrets->WiFiPassword);
		//io = new AdafruitIO_AIRLIFT(this->secrets->AdafruitIOUsername, this->secrets->AdafruitIOAccessKey, this->secrets->WiFiSSID, this->secrets->WiFiPassword, SPIWIFI_SS, NINA_ACK, NINA_RESETN, NINA_GPIO0, &SPIWIFI);
		

		// set up the feeds for the climate information.
		//temperatureFeed = io->feed("workshop-climate.temperature");
		////humidityFeed = io->feed("climate-testing.humidity");

		return this->Connect();
	}

	/// <summary>Connects to the WiFi network.</summary>
	InitializationResult AdafruitIOProxy::Connect()
	{
		Serial.println("Entered wifi connect");
		InitializationResult result;

		// Configure the pins used for the ESP32 connection
		#if !defined(SPIWIFI_SS)  // if the wifi definition isnt in the board variant
			// Don't change the names of these #define's! they match the variant ones
			#define SPIWIFI     SPI
			#define SPIWIFI_SS    4  // Chip select pin
			#define SPIWIFI_ACK   1   // a.k.a BUSY or READY pin
			#define ESP32_RESETN  0   // Reset pin
			#define ESP32_GPIO0   -1  // Not connected
		#endif

		io = new AdafruitIO_AIRLIFT(this->secrets->AdafruitIOUsername, this->secrets->AdafruitIOAccessKey, this->secrets->WiFiSSID, this->secrets->WiFiPassword, SPIWIFI_SS, SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);
		humidityFeed = io->feed("climate-testing.humidity");

		Serial.println("Defined pins...");

		// check for the WiFi module:
		WiFi.setPins(SPIWIFI_SS, SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);
		Serial.println("Set Pins...");

		if (WiFi.status() == WL_NO_MODULE)
		{
			result.ErrorMessage = F("Communication with WiFi module failed!");
			Serial.println(result.ErrorMessage);
			result.IsSuccessful = false;
			return result;
		}
		else
		{
			// Connect to WPA/WPA2 network
			//bool available = false;
			Serial.println("connecting to WiFi...");
			unsigned long starttime = millis();
			while ((millis() - starttime) <= AdafruitIOProxy::NetworkTimeoutMS)
			{
				status = WiFi.begin(this->secrets->WiFiSSID, this->secrets->WiFiPassword);
				if (status == WL_CONNECTED)
				{
					//available = true;
					this->IsConnected = true;
					break;
				}
			}

			//if (!available)
			if (!this->IsConnected)
			{
				// we exceeded our timeout period. return a failure.
				result.ErrorMessage = F("Failed to connect to WiFi network.");
				Serial.println(result.ErrorMessage);
				this->Disconnect();
				return result;
			}

			Serial.println(F("Connected to wifi"));
		}
		
		result.IsSuccessful = true;
		return result;
	}

	/// <summary>Disconnects from the WiFi network.</summary>
	void AdafruitIOProxy::Disconnect()
	{
		WiFi.disconnect();
		WiFi.end();
	}

	/// <summary>Connects to the WiFi network and transmits data to Adafruit IO.</summary>
	/// <param name="data">The <see cref="SensorData" to send to Adafruit IO.</param>
	IoTUploadResult AdafruitIOProxy::Transmit(SensorData data)
	{
		IoTUploadResult result;

		//Serial.println(F("connecting to wifi via Adafruit IO library"));
		//io->connect(); // this is just connecting to the wifi. it's just a wrapper to the underlying wifi system.

		// wait for a connection, but not forever yo! a few seconds should be good enough?
		// calling io->status() actually does a lot behind the scenes, and can return
		// quite a few different status's. We only care about being connected though.
		//bool available = false;
		//unsigned long starttime = millis();
		//while ((millis() - starttime) <= AdafruitIOProxy::NetworkTimeoutMS)
		//{
		//	/*if (io->status() == AIO_CONNECTED)
		//	{
		//		available = true;
		//		break;
		//	}*/
		//}

		//if (!available)
		//{
		//	// we exceeded our timeout period. return a failure.
		//	//result.ErrorMessage = io->statusText();
		//	this->Disconnect();
		//	return result;
		//}

		// send climate information to Adafruit IO
		//temperatureFeed->save(data.Climate.Temperature);
		humidityFeed->save(data.Climate.Humidity);

		Serial.println(F("sending data to Adafruit IO..."));

		// send the queued up data points. "run" commits the transaction (essentially).
		io->run();
		result.IsSuccess = true;

		// print humidity and temperature:
		Serial.print("Humidity: ");
		Serial.println(data.Climate.Humidity);
		Serial.print("Temperature: ");
		Serial.println(data.Climate.Temperature);

		// print the SSID of the network you're attached to:
		Serial.print("SSID: ");
		Serial.println(WiFi.SSID());

		// print your board's IP address:
		IPAddress ip = WiFi.localIP();
		Serial.print("IP Address: ");
		Serial.println(ip);

		// print the received signal strength:
		long rssi = WiFi.RSSI();
		Serial.print("signal strength (RSSI):");
		Serial.print(rssi);
		Serial.println(" dBm");

		Serial.println(F("data sent to adafruit!"));

		// get information about the wifi connection for use on the display
		result.SSID = WiFi.SSID();
		result.RSSI = WiFi.RSSI();
		result.LocalIP = WiFi.localIP();
		result.GatewayIP = WiFi.gatewayIP();
		result.SubnetMask = WiFi.subnetMask();
		result.ErrorMessage = F("");

		// disconnect from WiFi.
		//this->Disconnect();

		return result;
	}
}
