/*
https://io.adafruit.com/api/docs/

*/
#pragma once

#define USE_AIRLIFT

#include <SPI.h>
#include <WiFiNINA.h>

#include <Arduino.h>
#include <AdafruitIO_WiFi.h>

#include "..\Sensors\BME280Data.h"
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
		void Reset();
		bool IsConnected = false;
		IoTUploadResult Transmit(BME280Data data);
	private:
		/// <summary>The number of milliseconds we should wait for an internet connection to Adafruit IO.</summary>
		static const unsigned long NetworkTimeoutMS = 12000;
		/// <summary>The number of milliseconds we should wait for an internet connection to Adafruit IO.</summary>
		static const unsigned long AdafruitIOTouchTimeoutMS = 2000;
		/// <summary>The number of milliseconds we should delay the reboot of the network chip.</summary>
		static const unsigned long ResetChipDelayMS = 100;
		/// <summary>The pin used for Chip Select.</summary>
		static const uint8_t ChipSelectPin = 13;
		/// <summary>The pin used to notify us that the hardware is busy.</summary>
		static const uint8_t BusyPin = 11;
		/// <summary>The pin used for hardware reset.</summary>
		static const uint8_t ResetPin = 12;
		/// <summary>The pin used for reading output data from the chip.</summary>
		static const uint8_t GPIOPin = -1;

		AdafruitIO_WiFi *io;
		int status = WL_IDLE_STATUS;
		Secrets *secrets;

		AdafruitIO_Feed *temperatureFeed;
		AdafruitIO_Feed *humidityFeed;

		bool SaveData(BME280Data data);
		bool TouchAdafruitIO();
	};
}
