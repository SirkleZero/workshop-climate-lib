#pragma once

#include <Arduino.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>

#include "..\Configuration\Secrets.h"
#include "..\Sensors\BME280Data.h"
#include "..\InitializationResult.h"
#include "TXResult.h"
#include "SensorTransmissionResult.h"

using namespace Configuration;
using namespace Sensors;

namespace RFM69 {
	/// <summary>The class responsible for handling RFM69 hardware transmissions.</summary>
	class RFM69Proxy {
	public:
		/// <summary>The frequency at which the radio will operate.</summary>
		static const int16_t RadioFrequency = 915.0;
		static const int16_t TransmissionTimeout = 2000;
		static const uint8_t DefaultLEDPin = 13;

		RFM69Proxy(uint8_t address, int16_t radioFrequency = RFM69Proxy::RadioFrequency, uint8_t csPin = 8, uint8_t irqPin = 3, uint8_t rstPin = 4);
		RFM69Proxy(uint8_t address, int16_t radioFrequency = RFM69Proxy::RadioFrequency, uint8_t csPin = 8, uint8_t irqPin = 3, uint8_t rstPin = 4, uint8_t statusLEDPin = RFM69Proxy::DefaultLEDPin);

		InitializationResult Initialize();
		TXResult Transmit(BME280Data data);
		SensorTransmissionResult Listen();
		void Disable();
		void Enable();
		void Reset();
	private:
		int16_t radioFrequency;
		uint8_t address;
		uint8_t csPin;
		uint8_t irqPin;
		uint8_t rstPin;
		uint8_t statusLEDPin;

		RH_RF69 radio;
		RHReliableDatagram manager;

		void FlashStatusLED(byte delay_ms, byte loops);
	};
}
