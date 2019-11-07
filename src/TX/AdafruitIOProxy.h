/*
https://io.adafruit.com/api/docs/

*/
#ifndef AdafruitIOProxy_h
#define AdafruitIOProxy_h

#define USE_AIRLIFT

#include <SPI.h>
#include <WiFiNINA.h>

#include <Arduino.h>
#include <AdafruitIO_WiFi.h>

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
		bool Connect();
		void Disconnect();
		bool IsConnected = false;
		IoTUploadResult Transmit(SensorData data);
	private:
		/// <summary>The number of milliseconds we should wait for an internet connection to Adafruit IO.</summary>
		static const unsigned long NetworkTimeoutMS = 12000;
		static const unsigned long AdafruitIOTouchTimeoutMS = 2000;

		AdafruitIO_WiFi *io;
		int status = WL_IDLE_STATUS;
		Secrets *secrets;

		AdafruitIO_Feed *temperatureFeed;
		AdafruitIO_Feed *humidityFeed;

		bool SaveData(SensorData data);
		bool TouchAdafruitIO();
	};
}
#endif
