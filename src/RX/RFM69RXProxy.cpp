#include "RFM69RXProxy.h"

using namespace Sensors;

namespace RX {
	RFM69RXProxy::RFM69RXProxy() :
		rf69(this->RFM69_CS, this->RFM69_IRQ),
		manager(this->rf69, this->SERVER_ADDRESS)
	{}

	void RFM69RXProxy::Initialize()
	{
		//pinMode(LED, OUTPUT);     
		pinMode(this->RFM69_RST, OUTPUT);
		digitalWrite(this->RFM69_RST, LOW);

		// manual reset
		digitalWrite(this->RFM69_RST, HIGH);
		delay(10);
		digitalWrite(this->RFM69_RST, LOW);
		delay(10);

		if (!this->manager.init())
		{
			Serial.println(F("RFM69 radio init failed"));
			while (1);
		}

		// Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
		// No encryption
		if (!this->rf69.setFrequency(this->RF69_FREQ))
		{
			Serial.println(F("setFrequency failed"));
		}

		// If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
		// ishighpowermodule flag set like this:
		this->rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

		// The encryption key has to be the same as the one in the server
		uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
							0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
		this->rf69.setEncryptionKey(key);

		//pinMode(LED, OUTPUT);

		Serial.print(F("RFM69 radio @"));  Serial.print((int)this->RF69_FREQ);  Serial.println(F(" MHz"));
	}

	// NOTE: for some reason, that I don't understand exactly, this needs to sit here rather than with the class.
	// dumb on me.
	uint8_t acknowledgeData[25] = "acknowledged from node 1";

	SensorTransmissionResult RFM69RXProxy::Listen()
	{
		SensorTransmissionResult result;
		result.HasResult = false;

		if (this->manager.available())
		{
			// Wait for a message addressed to us from the client
			uint8_t messageBufferLength = sizeof(this->messageBuffer);
			uint8_t from;

			if (this->manager.recvfromAck(this->messageBuffer, &messageBufferLength, &from))
			{
				this->messageBuffer[messageBufferLength] = 0; // zero out (I don't really know why we do this to be honest)

				// deserialize the message sent into our common data structure
				SensorData messageData;
				memcpy(&messageData, this->messageBuffer, sizeof(messageData));
				result.Data = messageData;
				result.TransmissionSource = from;
				result.RSSI = this->rf69.lastRssi();
				result.HasResult = true;

				//blink LED 3 times, 40ms between blinks
				//Blink(LED, 40, 3);

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
