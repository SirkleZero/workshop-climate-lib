#include "RFM69Proxy.h"

namespace RFM69 {
	RFM69Proxy::RFM69Proxy(Devices myAddress, int16_t radioFrequency, uint8_t csPin, uint8_t irqPin, uint8_t rstPin) :
		RFM69Proxy::RFM69Proxy(myAddress, radioFrequency, csPin, irqPin, rstPin, RFM69Proxy::DefaultLEDPin)
	{}

	RFM69Proxy::RFM69Proxy(Devices myAddress, int16_t radioFrequency, uint8_t csPin, uint8_t irqPin, uint8_t rstPin, uint8_t statusLEDPin) :
		radio(csPin, irqPin),
		manager(this->radio, myAddress)
	{
		this->myAddress = myAddress;
		this->csPin = csPin;
		this->irqPin = irqPin;
		this->rstPin = rstPin;
		this->statusLEDPin = statusLEDPin;
	}

	/// <summary>Executes initialization logic for the object.</summary>
	/// <returns>An <see cref="InitializationResult"/> that describes the result of initialization.</returns>
	InitializationResult RFM69Proxy::Initialize()
	{
		InitializationResult result;

		// led indicator pin mode configuration.
		pinMode(this->statusLEDPin, OUTPUT);

		// set up the reset pin for the radio. The chip select and interrupt are set on the radio object.
		pinMode(this->rstPin, OUTPUT);

		// manual reset the radio.
		this->Reset();

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

		// disable the radio until we're actually going to use it.
		this->Disable();

		result.IsSuccessful = true;
		return result;
	}

	void RFM69Proxy::Disable()
	{
		// The CS Pin (#8) does not have a pullup resistor built in, so be sure to set this pin to HIGH when not using the 
		// radio! Will cause problems with other devices if we don't do this. 
		digitalWrite(this->csPin, HIGH);
	}

	void RFM69Proxy::Enable()
	{
		digitalWrite(this->csPin, LOW);
	}

	void RFM69Proxy::Reset()
	{
		digitalWrite(this->rstPin, LOW);
		delay(25);
		digitalWrite(this->rstPin, HIGH);
		delay(25);
		digitalWrite(this->rstPin, LOW);
		delay(25);

		this->Disable();
	}

	SensorTransmissionResult RFM69Proxy::ListenForBME280()
	{
		this->Enable();

		SensorTransmissionResult result;
		result.HasResult = false;

		if (this->manager.available())
		{
			uint8_t messageBuffer[RH_RF69_MAX_MESSAGE_LEN];

			// Wait for a message addressed to us from the sensor module
			uint8_t messageBufferLength = sizeof(messageBuffer);
			uint8_t from;

			if (this->manager.recvfromAck(messageBuffer, &messageBufferLength, &from))
			{
				messageBuffer[messageBufferLength] = 0; // ensure that the string is null terminated.

				// deserialize the message sent into our common data structure
				BME280Data messageData;
				memcpy(&messageData, messageBuffer, sizeof(messageData));
				result.Data = messageData;
				result.TransmissionSource = from;
				result.RSSI = this->radio.lastRssi();
				result.HasResult = true;

				this->FlashStatusLED(40, 3); //blink LED 3 times, 40ms between blinks

				// reply back to the originator sending client
				uint8_t acknowledgeData[RH_RF69_MAX_MESSAGE_LEN] = "acknowledged";
				if (!this->manager.sendtoWait(acknowledgeData, sizeof(acknowledgeData), from))
				{
					Serial.println(F("Sending failed (no acknowledgement packet)"));
				}
			}
		}

		this->Disable();

		return result;
	}

	/// <summary>Transmits the <see cref="BME280Data"/> data via the radio.</summary>
	/// <param name="data">The <see cref="BME280Data"/> to send.</param>
	TXResult RFM69Proxy::TransmitBME280(BME280Data data, uint8_t destinationAddress)
	{
		this->Enable();

		TXResult result;
		result.TransmitSuccessful = false;

		// copy the sensor data into a buffer that can be used to transmit the data to the server.
		byte transmissionBuffer[sizeof(data)] = { 0 };
		memcpy(transmissionBuffer, &data, sizeof(data));

		byte dataSize = sizeof(data);

		// Send a message to the node designated as the server
		if (this->manager.sendtoWait((uint8_t *)transmissionBuffer, dataSize, destinationAddress))
		{
			uint8_t acknowledgementBuffer[RH_RF69_MAX_MESSAGE_LEN];

			// Now wait for a reply from the server
			uint8_t acknowledgementBufferLength = sizeof(acknowledgementBuffer);
			uint8_t from;

			if (this->manager.recvfromAckTimeout(acknowledgementBuffer, &acknowledgementBufferLength, RFM69Proxy::TransmissionTimeout, &from))
			{
				acknowledgementBuffer[acknowledgementBufferLength] = 0; // make sure the string is null terminated.

				this->FlashStatusLED(40, 3); //blink LED 3 times, 40ms between blinks

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
			Serial.println(F("Sending failed (no acknowledgement packet)"));
		}

		this->Disable();

		return result;
	}

	void RFM69Proxy::FlashStatusLED(byte delay_ms, byte loops)
	{
		for (byte i = 0; i < loops; i++)
		{
			digitalWrite(this->statusLEDPin, HIGH);
			delay(delay_ms);
			digitalWrite(this->statusLEDPin, LOW);
			delay(delay_ms);
		}
	}
}
