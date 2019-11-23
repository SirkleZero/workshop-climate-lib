#pragma once

#include <Arduino.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>

#include "..\Configuration\Secrets.h"
#include "..\Sensors\BME280Data.h"
#include "..\InitializationResult.h"
#include "TXResult.h"

using namespace Configuration;
using namespace Sensors;

namespace TX {
	/// <summary>The class responsible for handling RFM69 hardware transmissions.</summary>
	class RFM69TXProxy {
	public:
		/// <summary>The frequency at which the radio will operate.</summary>
		static const int16_t RadioFrequency = 915.0;
		/// <summary>The address of the controller (server).</summary>
		static const byte ServerAddress = 1;
		/// <summary>The address of the sensor module.</summary>
		static const byte SensorModuleAddress = 2;
		/// <summary>The chip select pin address.</summary>

		RFM69TXProxy();

		InitializationResult Initialize();
		TXResult Transmit(BME280Data data);
	private:
		static const byte CSPin = 8;
		/// <summary>The interrupt pin address.</summary>
		static const byte INTPin = 3;
		/// <summary>The RST pin address.</summary>
		static const byte RSTPin = 4;
		/// <summary>The LED indicator pin address.</summary>
		static const byte LED = 13;

		RH_RF69 radio;
		RHReliableDatagram manager;

		uint8_t acknowledgementBuffer[RH_RF69_MAX_MESSAGE_LEN];

		void Blink(byte delay_ms, byte loops);
	};
}
