/*
A few notes on .run() as published by abachman on the adafruit.com forums:
https://forums.adafruit.com/viewtopic.php?f=56&t=156891&p=774110&hilit=timeout#p774110

.run() is where your sketch checks for new MQTT subscription messages and like brubell said, it's
where your device pings the MQTT broker once every 60s to keep the connection alive.

You've got a few options!

1. If you have no MQTT subscriptions you can skip calling it on every loop and only call it once per 
minute.That will make sure the PING happens on time and avoid the 100ms - ish delays.

2. If you're already publishing every 15 or so seconds and not subscribing to anything, you can leave 
it out entirely and the PUBLISH events happening when you call .save() will keep the connection alive. 
PING is the default, "I am active, but I have no new data to publish", but all the broker cares about 
is activity.

3. If you're expecting data but don't need it instantly, you can call it on a longer interval than "once 
every loop()".Maybe just call it on loops where you publish ?

4. Finally, the run() method in Adafruit IO Arduino takes an optional "busywait_timeout" argument 
which is the amount of time in ms to wait for new MQTT packets to come in.I'll let you guess what the 
default timeout is :D Sending a low value (e.g, 5) will make io.run() spend less time waiting, but 
might cause your sketch to miss some MQTT subscription messages.

If you peek under the hood, Adafruit IO Arduino's .run() method is pretty much only calling Adafruit MQTT's 
processPackets method, so if you change libraries you'll end up with a similar problem but be a little closer 
to the truth of what's happening.

*/

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
			#define SPIWIFI			SPI
			#define SPIWIFI_SS		AdafruitIOProxy::ChipSelectPin  // Chip select pin
			#define SPIWIFI_ACK		AdafruitIOProxy::BusyPin   // a.k.a BUSY or READY pin
			#define ESP32_RESETN	AdafruitIOProxy::ResetPin   // Reset pin
			#define ESP32_GPIO0		AdafruitIOProxy::GPIOPin  // Not connected
		#endif

		// NOTE: This was the only way I could figure out how to pass in non-hardcoded values to the Adafruit library. These values are read from a JSON document to make them easily configurable.
		// Make sure to delete the io object in the destructor!
		io = new AdafruitIO_AIRLIFT
		(
			this->secrets->AdafruitIOUsername, 
			this->secrets->AdafruitIOAccessKey, 
			this->secrets->WiFiSSID, 
			this->secrets->WiFiPassword, 
			SPIWIFI_SS,
			SPIWIFI_ACK, 
			ESP32_RESETN, 
			ESP32_GPIO0, 
			&SPIWIFI
		);

		// set up the mqtt feeds we are publishing to
		humidityFeed = io->feed(this->secrets->HumidityFeed);
		temperatureFeed = io->feed(this->secrets->TemperatureFeed);

		// set the pins of our ESP32 Airlift
		WiFi.setPins
		(
			SPIWIFI_SS, 
			SPIWIFI_ACK, 
			ESP32_RESETN, 
			ESP32_GPIO0, 
			&SPIWIFI
		);

		// check to make sure we have a WiFi module. If the pins aren't correctly wired up or configured
		// our module won't be recognized and we won't be able to process anything WiFi related.
		if (WiFi.status() == WL_NO_MODULE)
		{
			result.ErrorMessage = F("Communication with WiFi module failed!");
			Serial.println(result.ErrorMessage);
			result.IsSuccessful = false;
			return result;
		}

		// set the network hostname for the monitor
		WiFi.setHostname(this->secrets->Hostname);

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
				this->IsConnected = true;
				Serial.println(F("Connected to WiFi."));
				return this->IsConnected;
			}
		}

		Serial.println(F("Failed to connect to WiFi network!"));
		this->Disconnect();

		return this->IsConnected;
	}

	bool AdafruitIOProxy::TouchAdafruitIO()
	{
		// Calling mqttStatus() attempts to connect to the mqtt broker. Calling .run() uses the
		// call to mqttStatus() as part of its logic, but unfortunately does so in a potentially infinate loop.
		// So instead of letting .run() run away from us in that loop, we'll execute that same logic 
		// inside a timer based loop to ensure connectivity. If we get a connection, we'll call .run()
		// so it can continue doing what it does from a keep alive standpoint; otherwise we'll exit this
		// function with a false value, indicating that we were unable to connect to the broker in
		// the allowed time. This function is essentially a work-around to prevent us from having infinate
		// loops in .run(). If we get an infinate loop, all processing will stop, hanging our device. If using
		// a Watchdog timer like I do in this application, the device will ultimately reboot when the timer
		// lapses.

		// TODO: Issue a pull request to add an overload to .run() that would accept a timeout value. Currently 
		// .run() has the following line of code that can result in infinate loop: while(mqttStatus() != AIO_CONNECTED){}
		Serial.println("Connecting to Adafruit IO...");
		unsigned long starttime = millis();
		while ((millis() - starttime) <= AdafruitIOProxy::AdafruitIOTouchTimeoutMS)
		{
			// once we establish connection using mqttStatus(), execute .run().
			if (io->mqttStatus() == AIO_CONNECTED)
			{
				io->run();
				Serial.println("Connected to Adafruit IO.");
				return true;
			}
		}

		Serial.println("Failed to connect to Adafruit IO.");
		return false;
	}

	/// <summary>Disconnects from the WiFi network.</summary>
	void AdafruitIOProxy::Disconnect()
	{
		this->IsConnected = false;
		WiFi.disconnect();
		WiFi.end();
	}

	/// <summary></summary>
	void AdafruitIOProxy::Reset()
	{
		// TODO: This needs to be fully tested!
		digitalWrite(AdafruitIOProxy::ResetPin, LOW);
		delay(AdafruitIOProxy::ResetChipDelayMS);
		digitalWrite(AdafruitIOProxy::ResetPin, HIGH);
	}

	/// <summary>Connects to the WiFi network and transmits data to Adafruit IO.</summary>
	/// <param name="data">The <see cref="SensorData" to send to Adafruit IO.</param>
	IoTUploadResult AdafruitIOProxy::Transmit(BME280Data data)
	{
		// print humidity and temperature:
		/*Serial.print("Humidity: ");
		Serial.println(data.Humidity);
		Serial.print("Temperature: ");
		Serial.println(data.Temperature);*/

		IoTUploadResult result;

		// double check that we are actually connected to wifi.
		this->status = WiFi.status();
		if (this->status != WL_CONNECTED)
		{
			// we are unable to connect to wifi. fail gracefully.
			if (!this->Connect())
			{
				// save the data for when the connection comes back.
				this->SaveData(data);

				result.ErrorMessage = F("Failed to reconnect to WiFi, exiting Transmit().");
				this->Disconnect();
				return result;
			}
		}

		// If we got to this point, we know we are connected to a WiFi network. Let's ensure our
		// connection to Adafruit IO by executing our timer constrained call to .run().
		if (!this->TouchAdafruitIO())
		{
			result.ErrorMessage = F("Failed to successfully ping Adafruit IO.");
			return result;
		}

		// Send the data to Adafruit IO. If the connection is solid, then this will send directly; 
		// otherwise it will queue up and be sent when the connection becomes available again.
		if (!this->SaveData(data))
		{
			result.ErrorMessage = F("Failed to queue or otherwise save data to Adafruit IO.");
			return result;
		}

		Serial.println(F("Data transmission complete."));
		result.IsSuccess = true;
		return result;
	}

	bool AdafruitIOProxy::SaveData(BME280Data data)
	{
		// TODO: Queue up our data, just in case we aren't connected, then
		// send it once we are. For now, what we have here is good enough for
		// us to reliably send data. The queue will make it just a bit more
		// robust.
		return humidityFeed->save(data.Humidity) && temperatureFeed->save(data.Temperature);
		//return true; // for testing purposes
	}
}
