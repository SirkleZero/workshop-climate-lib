#ifndef RFM69RXProxy_h
#define RFM69RXProxy_h

#include <Arduino.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>
#include "SensorTransmissionResult.h"
#include "..\Configuration\Secrets.h"
#include "..\InitializationResult.h"
#include "..\Sensors\BME280Data.h"

using namespace Configuration;
using namespace Sensors;

namespace RX {
	/// <summary>Class that is responsible for handling the reception functionality of a RFM69 radio.</summary>
	class RFM69RXProxy {
	public:
		/// <summary>The frequency at which the radio will operate.</summary>
		static const int16_t RadioFrequency = 915.0;
		/// <summary>The address of the controller (server).</summary>
		static const byte ServerAddress = 1;

		RFM69RXProxy();

		InitializationResult Initialize();
		SensorTransmissionResult Listen();
		InitializationResult Reset();
	private:		
		/// <summary>The chip select pin address.</summary>
		static const byte CSPin = 12;
		/// <summary>The RST pin address.</summary>
		static const byte RSTPin = 11;
		/// <summary>The IRQ pin address.</summary>
		static const byte IRQPin = 13;

		RH_RF69 radio;
		RHReliableDatagram manager;

		uint8_t messageBuffer[RH_RF69_MAX_MESSAGE_LEN];
	};
}
#endif
