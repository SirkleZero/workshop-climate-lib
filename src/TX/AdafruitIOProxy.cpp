// a few notes on .run() as published by abachman on the adafruit.com forums: https://forums.adafruit.com/viewtopic.php?f=56&t=156891&p=774110&hilit=timeout#p774110

//.run() is where your sketch checks for new MQTT subscription messages and like brubell said, it's where your device pings the MQTT broker once every 60s to keep the connection alive.
//
//You've got a few options!
//
//1. If you have no MQTT subscriptions you can skip calling it on every loop and only call it once per minute.That will make sure the PING happens on time and avoid the 100ms - ish delays.
//
//2. If you're already publishing every 15 or so seconds and not subscribing to anything, you can leave it out entirely and the PUBLISH events happening when you call .save() will keep the connection alive. PING is the default, "I am active, but I have no new data to publish", but all the broker cares about is activity.
//
//3. If you're expecting data but don't need it instantly, you can call it on a longer interval than "once every loop()".Maybe just call it on loops where you publish ?
//
//4. Finally, the run() method in Adafruit IO Arduino takes an optional "busywait_timeout" argument which is the amount of time in ms to wait for new MQTT packets to come in.I'll let you guess what the default timeout is :D Sending a low value (e.g, 5) will make io.run() spend less time waiting, but might cause your sketch to miss some MQTT subscription messages.
//
//If you peek under the hood, Adafruit IO Arduino's .run() method is pretty much only calling Adafruit MQTT's processPackets method, so if you change libraries you'll end up with a similar problem but be a little closer to the truth of what's happening.

#include "AdafruitIOProxy.h"

using namespace Sensors;
using namespace Configuration;

namespace TX {
	/// <summary>Initializes a new instance of the <see cref="AdafruitIOProxy"/> class.</summary>
	AdafruitIOProxy::AdafruitIOProxy() {}

	/// <summary>Destroys an existing instance of the <see cref="AdafruitIOProxy"/> class.</summary>
	AdafruitIOProxy::~AdafruitIOProxy()
	{
		if(io)
		{
			delete io;
		}
	}

	/// <summary>Executes initialization logic for the object.</summary>
	/// <param name="secrets">The <see cref="Secrets"/> that contain information for connecting to the WiFi network and Adafruit IO services.</param>
	/// <returns>An <see cref="InitializationResult"/> that describes the result of initialization.</returns>
	InitializationResult AdafruitIOProxy::Initialize(Secrets *secrets)
	{
		InitializationResult result;
		this->secrets = secrets;

		// Configure the pins used for the ESP32 connection
		#if !defined(SPIWIFI_SS)  // if the wifi definition isnt in the board variant
			// Don't change the names of these #define's! they match the variant ones
			#define SPIWIFI     SPI
			#define SPIWIFI_SS    4  // Chip select pin
			#define SPIWIFI_ACK   1   // a.k.a BUSY or READY pin
			#define ESP32_RESETN  0   // Reset pin
			#define ESP32_GPIO0   -1  // Not connected
		#endif

		// NOTE: This was the only way I could figure out how to pass in non-hardcoded values to the Adafruit library. These values are read from a JSON document to make them easily configurable.
		// Make sure to delete the io object in the destructor!
		io = new AdafruitIO_AIRLIFT(this->secrets->AdafruitIOUsername, this->secrets->AdafruitIOAccessKey, this->secrets->WiFiSSID, this->secrets->WiFiPassword, SPIWIFI_SS, SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);

		// set up the mqtt feeds we are publishing to
		humidityFeed = io->feed("climate-testing.humidity");

		// set the pins of our ESP32 Airlift
		WiFi.setPins(SPIWIFI_SS, SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);

		// check to make sure we have a WiFi module. If the pins aren't correctly wired up or configured
		// our module won't be recognized and we won't be able to process anything WiFi related.
		if (WiFi.status() == WL_NO_MODULE)
		{
			result.ErrorMessage = F("Communication with WiFi module failed!");
			Serial.println(result.ErrorMessage);
			result.IsSuccessful = false;
			return result;
		}

		result.IsSuccessful = true;
		return result;
	}

	/// <summary>Connects to the WiFi network.</summary>
	bool AdafruitIOProxy::Connect()
	{
		// Connect to WPA/WPA2 network
		Serial.println("Connecting to WiFi...");
		unsigned long starttime = millis();
		while ((millis() - starttime) <= AdafruitIOProxy::NetworkTimeoutMS)
		{
			this->status = WiFi.begin(this->secrets->WiFiSSID, this->secrets->WiFiPassword);
			if (this->status == WL_CONNECTED)
			{
				// establish a connection to adafruit io
				io->run();

				this->IsConnected = true;
				Serial.println(F("Connected to wifi"));
				return this->IsConnected;
			}
		}

		Serial.println(F("Failed to connect to WiFi network."));
		this->Disconnect();

		return this->IsConnected;
	}

	/// <summary>Disconnects from the WiFi network.</summary>
	void AdafruitIOProxy::Disconnect()
	{
		// TODO: Is this where I should kick the reset pin of the chip?
		this->IsConnected = false;
		WiFi.disconnect();
		WiFi.end();
	}

	/// <summary>Connects to the WiFi network and transmits data to Adafruit IO.</summary>
	/// <param name="data">The <see cref="SensorData" to send to Adafruit IO.</param>
	IoTUploadResult AdafruitIOProxy::Transmit(SensorData data)
	{
		// print humidity and temperature:
		Serial.print("Humidity: ");
		Serial.println(data.Climate.Humidity);
		Serial.print("Temperature: ");
		Serial.println(data.Climate.Temperature);

		IoTUploadResult result;

		// double check that we are actually connected to wifi.
		this->status = WiFi.status();
		if (this->status != WL_CONNECTED)
		{
			// we are unable to connect to wifi. fail gracefully.
			if (!this->Connect())
			{
				// queue the data for when the connection comes back.
				this->QueueData(data);

				result.IsSuccess = false;
				result.ErrorMessage = F("Failed to reconnect to WiFi, exiting Transmit().");
				//result.ErrorMessage = io->statusText();
				this->Disconnect();
				return result;
			}
		}

		// Queue the data that will be sent to Adafruit IO.
		this->QueueData(data);

		// send the queued up data points. "run" commits the transaction (essentially).
		// here we are using our network timeout that is a value less than the watchdog
		// timer that will restart the device once that timespan has elapsed.
		// TODO: wrap run in a timer... if the time elapses, then immediate return so
		// the arduino sketch can reset the watchdog timer. This should allow things to
		// queue until a reliable connection is made.
		Serial.print("Adafruit IO Status: ");
		Serial.println(io->statusText());
		Serial.print("WiFi Status: ");
		Serial.println(WiFi.status());

		unsigned long starttime = millis();
		while ((millis() - starttime) <= 5000)
		{
			if (io->status() == AIO_CONNECTED)
			{
				Serial.println(F("Sending data to Adafruit IO..."));
				//io->run();
				Serial.println(F("Data sent to adafruit!"));
				result.IsSuccess = true;
				break;
			}
		}

		return result;
	}

	void AdafruitIOProxy::QueueData(SensorData data)
	{
		humidityFeed->save(data.Climate.Humidity);
	}
}
