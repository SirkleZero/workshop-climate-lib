#include "RFM69RXProxy.h"

using namespace Sensors;

namespace RX {
	/// <summary>Initializes a new instance of the <see cref="RFM69RXProxy"/> class.</summary>
	RFM69RXProxy::RFM69RXProxy() :
		radio(RFM69RXProxy::CSPin, RFM69RXProxy::IRQPin),
		manager(this->radio, RFM69RXProxy::ServerAddress)
	{}

	/// <summary>Executes initialization logic for the object.</summary>
	/// <returns>An <see cref="InitializationResult"/> that describes the result of initialization.</returns>
	InitializationResult RFM69RXProxy::Initialize()
	{
		InitializationResult result;

		// set up the reset pin
		pinMode(RFM69RXProxy::RSTPin, OUTPUT);
		digitalWrite(RFM69RXProxy::RSTPin, LOW);

		// manually reset the radio. Not exactly sure why we do this, but it's in the example sketch...
		digitalWrite(RFM69RXProxy::RSTPin, HIGH);
		delay(25);
		digitalWrite(RFM69RXProxy::RSTPin, LOW);
		delay(25);

		// check to make sure the radio has properly initialized.
		if (!this->manager.init())
		{
			result.ErrorMessage = F("RFM69 radio initialization failed.");
			return result;
		}

		// Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module). No encryption
		if (!this->radio.setFrequency(RFM69RXProxy::RadioFrequency))
		{
			result.ErrorMessage = F("Call to setFrequency() failed.");
			return result;
		}

		// If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
		// ishighpowermodule flag set like this:
		this->radio.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

		// set the encryption key
		this->radio.setEncryptionKey(Secrets::RadioEncryptionKey);

		result.IsSuccessful = true;
		return result;
	}

	InitializationResult RFM69RXProxy::Reset()
	{
		return this->Initialize();
	}

	// NOTE: for some reason, that I don't understand exactly, this needs to sit here rather than with the class. dumb on me.
	uint8_t acknowledgeData[25] = "acknowledged from node 1";

	/// <summary>Listens for a transmission from the sensor module.</summary>
	/// <remarks>This method should be run as frequently as possible.</remarks>
	SensorTransmissionResult RFM69RXProxy::Listen()
	{
		SensorTransmissionResult result;
		result.HasResult = false;

		if (this->manager.available())
		{
			// Wait for a message addressed to us from the sensor module
			uint8_t messageBufferLength = sizeof(this->messageBuffer);
			uint8_t from;

			if (this->manager.recvfromAck(this->messageBuffer, &messageBufferLength, &from))
			{
				this->messageBuffer[messageBufferLength] = 0; // ensure that the string is null terminated.

				// deserialize the message sent into our common data structure
				BME280Data messageData;
				memcpy(&messageData, this->messageBuffer, sizeof(messageData));
				result.Data = messageData;
				result.TransmissionSource = from;
				result.RSSI = this->radio.lastRssi();
				result.HasResult = true;

				// Send a reply back to the originator client
				if (!this->manager.sendtoWait(acknowledgeData, sizeof(acknowledgeData), from))
				{
					Serial.println(F("Sending failed (no ack)"));
				}
			}
		}

		return result;
	}
}
