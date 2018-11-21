#include "RFM69RXProxy.h"

using namespace Sensors;

namespace RX {
	/// <summary>Initializes a new instance of the <see cref="RFM69RXProxy"/> class.</summary>
	RFM69RXProxy::RFM69RXProxy() :
		radio(this->CSPin, this->IRQPin),
		manager(this->radio, this->ServerAddress)
	{}

	/// <summary>Executes initialization logic for the object.</summary>
	void RFM69RXProxy::Initialize()
	{
		// set up the reset pin
		pinMode(this->RSTPin, OUTPUT);
		digitalWrite(this->RSTPin, LOW);

		// manually reset the radio. Not exactly sure why we do this, but it's in the example sketch...
		digitalWrite(this->RSTPin, HIGH);
		delay(10);
		digitalWrite(this->RSTPin, LOW);
		delay(10);

		// check to make sure the radio has properly initialized.
		if (!this->manager.init())
		{
			Serial.println(F("RFM69 radio init failed"));
			while (1); //TODO: if the radio fails, for whatever reason, I suspect that this here line of code is haunting us. Instrument this!!!
		}

		// Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module). No encryption
		if (!this->radio.setFrequency(this->RadioFrequency))
		{
			Serial.println(F("setFrequency failed"));
		}

		// If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
		// ishighpowermodule flag set like this:
		this->radio.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

		// The encryption key has to be the same as the one in the server
		uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
							0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
		this->radio.setEncryptionKey(key);

		Serial.print(F("RFM69 radio @"));  Serial.print((int)this->RadioFrequency);  Serial.println(F(" MHz"));
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
				SensorData messageData;
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
