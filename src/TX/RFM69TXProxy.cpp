#include "RFM69TXProxy.h"

using namespace Sensors;

namespace TX {
	/// <summary>Initializes a new instance of the <see cref="RFM69TXProxy"/> class.</summary>
	RFM69TXProxy::RFM69TXProxy() :
		radio(this->CSPin, this->INTPin),
		manager(this->radio, this->SensorModuleAddress)
	{}

	/// <summary>Executes initialization logic for the object.</summary>
	/// <returns>An <see cref="InitializationResult"/> that describes the result of initialization.</returns>
	InitializationResult RFM69TXProxy::Initialize()
	{
		InitializationResult result;

		// led indicator pin mode configuration.
		pinMode(this->LED, OUTPUT);

		// set up the pins for the radio.
		pinMode(this->LED, OUTPUT);
		pinMode(this->RSTPin, OUTPUT);
		digitalWrite(this->RSTPin, LOW);

		// manual reset the radio.
		digitalWrite(this->RSTPin, HIGH);
		delay(25);
		digitalWrite(this->RSTPin, LOW);
		delay(25);

		if (!this->manager.init())
		{
			result.ErrorMessage = F("RFM69 radio init failed");
			return result;
		}

		// Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module). No encryption
		if (!this->radio.setFrequency(RadioFrequency))
		{
			result.ErrorMessage = F("setFrequency failed");
			return result;
		}

		// If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
		// ishighpowermodule flag set like this:
		this->radio.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

		this->radio.setEncryptionKey(Secrets::RadioEncryptionKey);

		result.IsSuccessful = true;
		return result;
	}

	// NOTE: for some reason, that I don't understand exactly, this needs to sit here rather than with the class.
	// dumb on me.
	uint8_t data[] = "  OK";

	/// <summary>Transmits the <see cref="BME280Data"/> data via the radio.</summary>
	/// <param name="data">The <see cref="BME280Data"/> to send.</param>
	TXResult RFM69TXProxy::Transmit(BME280Data data)
	{
		TXResult result;
		result.TransmitSuccessful = false;

		// copy the sensor data into a buffer that can be used to transmit the data to the server.
		byte transmissionBuffer[sizeof(data)] = { 0 };
		memcpy(transmissionBuffer, &data, sizeof(data));

		byte dataSize = sizeof(data);

		// Send a message to the node designated as the server
		if (this->manager.sendtoWait((uint8_t *)transmissionBuffer, dataSize, this->ServerAddress))
		{
			// Now wait for a reply from the server
			uint8_t acknowledgementBufferLength = sizeof(this->acknowledgementBuffer);
			uint8_t from;

			if (this->manager.recvfromAckTimeout(this->acknowledgementBuffer, &acknowledgementBufferLength, 2000, &from))
			{
				this->acknowledgementBuffer[acknowledgementBufferLength] = 0; // make sure the string is null terminated.

				Blink(40, 3); //blink LED 3 times, 40ms between blinks

				result.From = from;
				result.RSSI = this->radio.lastRssi();
				result.TransmitSuccessful = true;
			}
			else
			{
				Serial.println(F("No reply, is anyone listening?"));
			}
		}
		else
		{
			Serial.println(F("Sending failed (no ack)"));
		}

		return result;
	}

	void RFM69TXProxy::Blink(byte delay_ms, byte loops)
	{
		for (byte i = 0; i < loops; i++)
		{
			digitalWrite(this->LED, HIGH);
			delay(delay_ms);
			digitalWrite(this->LED, LOW);
			delay(delay_ms);
		}
	}
}
