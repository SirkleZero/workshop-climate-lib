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
		io = new AdafruitIO_WINC1500(this->secrets->AdafruitIOUsername, this->secrets->AdafruitIOAccessKey, this->secrets->WiFiSSID, this->secrets->WiFiPassword);

		// set up the feeds for the climate information.
		temperatureFeed = io->feed("workshop-climate.temperature");
		humidityFeed = io->feed("workshop-climate.humidity");
		pressureFeed = io->feed("workshop-climate.pressure");

		// set up the feeds for the particulate information.
		pm10_standard = io->feed("workshop-climate.pm10-standard");
		pm25_standard = io->feed("workshop-climate.pm25-standard");
		pm100_standard = io->feed("workshop-climate.pm100-standard");
		pm10_environment = io->feed("workshop-climate.pm10-environment");
		pm25_environment = io->feed("workshop-climate.pm25-environment");
		pm100_environment = io->feed("workshop-climate.pm100-environment");
		particles_03um = io->feed("workshop-climate.particles-03um");
		particles_05um = io->feed("workshop-climate.particles-05um");
		particles_10um = io->feed("workshop-climate.particles-10um");
		particles_25um = io->feed("workshop-climate.particles-25um");
		particles_50um = io->feed("workshop-climate.particles-50um");
		particles_100um = io->feed("workshop-climate.particles-100um");

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

		Serial.println(F("connecting to wifi via Adafruit IO library"));
		io->connect(); // this is just connecting to the wifi. it's just a wrapper to the underlying wifi system.

		// wait for a connection, but not forever yo! 15 seconds should be good enough?
		// calling io->status() actually does a lot behind the scenes, and can return
		// quite a few different status's. We only care about being connected though.
		//byte secondsToWait = 15;
		//unsigned long connectionDelay = 500;
		float iterations = AdafruitIOProxy::SecondsToWait * (1000 / AdafruitIOProxy::ConnectionDelay);
		while (io->status() < AIO_CONNECTED && iterations > 0)
		{
			Serial.print(F("."));
			delay(AdafruitIOProxy::ConnectionDelay);
			iterations--;
		}

		if (iterations <= 0)
		{
			// we exceeded our timeout period. return a failure.
			result.ErrorMessage = io->statusText();
			this->Disconnect();
			return result;
		}

		// send climate information to Adafruit IO
		temperatureFeed->save(data.Climate.Temperature);
		humidityFeed->save(data.Climate.Humidity);
		pressureFeed->save(data.Climate.Pressure / 100); // send it in hPa

		// send particulates information to Adafruit IO
		pm10_standard->save(data.Particulates.pm10_standard);
		pm25_standard->save(data.Particulates.pm25_standard);
		pm100_standard->save(data.Particulates.pm100_standard);
		pm10_environment->save(data.Particulates.pm10_env);
		pm25_environment->save(data.Particulates.pm25_env);
		pm100_environment->save(data.Particulates.pm100_env);
		particles_03um->save(data.Particulates.particles_03um);
		particles_05um->save(data.Particulates.particles_05um);
		particles_10um->save(data.Particulates.particles_10um);
		particles_25um->save(data.Particulates.particles_25um);
		particles_50um->save(data.Particulates.particles_50um);
		particles_100um->save(data.Particulates.particles_100um);

		Serial.println(F("sending data to Adafruit IO..."));

		// send the queued up data points. "run" commits the transaction (essentially).
		io->run();
		result.IsSuccess = true;

		Serial.println(F("data sent to adafruit!"));

		// get information about the wifi connection for use on the display
		result.SSID = WiFi.SSID();
		result.RSSI = WiFi.RSSI();
		result.LocalIP = WiFi.localIP();
		result.GatewayIP = WiFi.gatewayIP();
		result.SubnetMask = WiFi.subnetMask();
		result.ErrorMessage = F("");

		// disconnect from WiFi.
		this->Disconnect();

		return result;
	}
}
