#include "AdafruitIOProxy.h"

using namespace Sensors;
using namespace Configuration;

namespace TX {
	/// <summary>Initializes a new instance of the <see cref="BME280Proxy"/> class.</summary>
	AdafruitIOProxy::AdafruitIOProxy() {}

	/// <summary>Destroys an existing instance of the <see cref="BME280Proxy"/> class.</summary>
	AdafruitIOProxy::~AdafruitIOProxy()
	{
		delete io;
	}

	/// <summary>Executes initialization logic for the object.</summary>
	/// <param name="secrets">The <see cref="Secrets"/> that contain information for connecting to the WiFi network and Adafruit IO services.</param>
	/// <returns>An <see cref="InitializationResult"/> that describes the result of initialization.</returns>
	InitializationResult AdafruitIOProxy::Initialize(Secrets *secrets)
	{
		InitializationResult result;
		this->secrets = secrets;

		// NOTE: this really was the only way I could actually get this to work. Make sure to delete the io object in the destructor!
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

		Serial.println("Define pins...");
		WiFi.setPins(SPIWIFI_SS, SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);
		Serial.println("Pins are set...");

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
		Serial.println(F("Sending data to Adafruit IO..."));
		io->run();
		Serial.println(F("Data sent to adafruit!"));
		result.IsSuccess = true;

		unsigned long starttime = millis();
		while ((millis() - starttime) <= AdafruitIOProxy::NetworkTimeoutMS)
		{
			if (io->status() == AIO_CONNECTED)
			{
				break;
			}
		}

		// print humidity and temperature:
		Serial.print("Humidity: ");
		Serial.println(data.Climate.Humidity);
		Serial.print("Temperature: ");
		Serial.println(data.Climate.Temperature);

		return result;
	}

	void AdafruitIOProxy::QueueData(SensorData data)
	{
		// behind the scenes, these save methods actually queue data to be processed
		// by mqtt.
		humidityFeed->save(data.Climate.Humidity);
	}
}
