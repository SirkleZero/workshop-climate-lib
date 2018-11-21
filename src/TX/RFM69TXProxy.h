#ifndef RFM69TXProxy_h
#define RFM69TXProxy_h

#include <Arduino.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>

#include "..\Sensors\SensorData.h"
#include "TXResult.h"

using namespace Sensors;

namespace TX {
	/// <summary>The class responsible for handling RFM69 hardware transmissions.</summary>
	class RFM69TXProxy {
	public:
		RFM69TXProxy();

		void Initialize();
		TXResult Transmit(SensorData data);
	private:
		/// <summary>The frequency at which the radio will operate.</summary>
		const int16_t RadioFrequency = 915.0;
		/// <summary>The address of the controller (server).</summary>
		const byte ServerAddress = 1;
		/// <summary>The address of the sensor module.</summary>
		const byte SensorModuleAddress = 2;
		/// <summary>The chip select pin address.</summary>
		const byte CSPin = 8;
		/// <summary>The interrupt pin address.</summary>
		const byte INTPin = 3;
		/// <summary>The RST pin address.</summary>
		const byte RSTPin = 4;
		/// <summary>The LED indicator pin address.</summary>
		const byte LED = 13;

		RH_RF69 radio;
		RHReliableDatagram manager;

		uint8_t acknowledgementBuffer[RH_RF69_MAX_MESSAGE_LEN];

		void Blink(byte delay_ms, byte loops);
	};
}
#endif
