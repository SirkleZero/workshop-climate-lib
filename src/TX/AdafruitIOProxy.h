/*
https://io.adafruit.com/api/docs/

*/
#ifndef AdafruitIOProxy_h
#define AdafruitIOProxy_h

#include <Arduino.h>
#include <AdafruitIO_WiFi.h>
#include <WiFi101.h>

#include "..\Sensors\SensorData.h"
#include "..\Configuration\Secrets.h"
#include "..\InitializationResult.h"
#include "IoTUploadResult.h"

using namespace Sensors;
using namespace Configuration;

namespace TX {
	/// <summary>Represents a class that knows how to interact with Adafruit IO.</summary>
	class AdafruitIOProxy {
	public:
		AdafruitIOProxy();
		~AdafruitIOProxy();
		InitializationResult Initialize(Secrets *secrets);
		IoTUploadResult Transmit(SensorData data);
	private:
		/// <summary>The number of milliseconds we should wait for an internet connection to Adafruit IO.</summary>
		static const byte NetworkTimeoutMS = 15000;

		Secrets *secrets;
		AdafruitIO_WiFi *io;

		AdafruitIO_Feed *temperatureFeed;
		AdafruitIO_Feed *humidityFeed;
		AdafruitIO_Feed *pressureFeed;

		AdafruitIO_Feed *pm10_standard;
		AdafruitIO_Feed *pm25_standard;
		AdafruitIO_Feed *pm100_standard;
		AdafruitIO_Feed *pm10_environment;
		AdafruitIO_Feed *pm25_environment;
		AdafruitIO_Feed *pm100_environment;
		AdafruitIO_Feed *particles_03um;
		AdafruitIO_Feed *particles_05um;
		AdafruitIO_Feed *particles_10um;
		AdafruitIO_Feed *particles_25um;
		AdafruitIO_Feed *particles_50um;
		AdafruitIO_Feed *particles_100um;

		void Disconnect();
	};
}
#endif
