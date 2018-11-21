#ifndef RFM69RXProxy_h
#define RFM69RXProxy_h

#include <Arduino.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>
#include "SensorTransmissionResult.h"
#include "..\Sensors\SensorData.h"

using namespace Sensors;

namespace RX {
	/// <summary></summary>
	class RFM69RXProxy {
	public:
		RFM69RXProxy();

		void Initialize();
		SensorTransmissionResult Listen();
	private:
		/// <summary>The frequency at which the radio will operate.</summary>
		const int16_t RadioFrequency = 915.0;
		/// <summary>The address of the controller (server).</summary>
		const byte ServerAddress = 1;
		/// <summary>The chip select pin address.</summary>
		const byte CSPin = 12;
		/// <summary>The RST pin address.</summary>
		const byte RSTPin = 11;
		/// <summary>The IRQ pin address.</summary>
		const byte IRQPin = 13;

		RH_RF69 radio;
		RHReliableDatagram manager;

		uint8_t messageBuffer[RH_RF69_MAX_MESSAGE_LEN];
	};
}
#endif
