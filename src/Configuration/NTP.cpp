#include "NTP.h"

namespace Configuration {
	NTP::NTP() : 
		timeServer(1, 1, 1, 1)
	{
	}

	/// <summary>Executes initialization logic for the object.</summary>
	/// <param name="secrets">The <see cref="Secrets"/> that contain information for connecting to the WiFi network.</param>
	/// <returns>An <see cref="InitializationResult"/> that describes the result of initialization.</returns>
	InitializationResult NTP::Initialize(Secrets *secrets)
	{
		InitializationResult result;
		this->secrets = secrets;

		WiFi.setPins(8, 7, 4, 2);

		setSyncProvider(this->getNtpTime);
		//setSyncInterval(60);

		result.IsSuccessful = true;
		return result;
	}

	const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
	byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

	time_t NTP::getNtpTime()
	{
		WiFi.begin(this->secrets->WiFiSSID, this->secrets->WiFiPassword);

		while (WiFi.status() != WL_CONNECTED) {
			delay(500);
			Serial.print(".");
		}

		Udp.begin(localPort);
		WiFi.hostByName(ntpServerName, timeServer);

		while (Udp.parsePacket() > 0); // discard any previously received packets

		Serial.println("Transmit NTP Request");
		sendNTPpacket(this->timeServer);
		uint32_t beginWait = millis();

		while (millis() - beginWait < 1500) {
			int size = Udp.parsePacket();

			if (size >= NTP_PACKET_SIZE) {
				Serial.println("Receive NTP Response");
				Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
				unsigned long secsSince1900;

				// convert four bytes starting at location 40 to a long integer
				secsSince1900 = (unsigned long)packetBuffer[40] << 24;
				secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
				secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
				secsSince1900 |= (unsigned long)packetBuffer[43];

				WiFi.disconnect();

				return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
			}
		}

		WiFi.disconnect();

		Serial.println("No NTP Response :-(");
		return 0; // return 0 if unable to get the time
	}

	// send an NTP request to the time server at the given address
	void NTP::sendNTPpacket(IPAddress &address)
	{
		// set all bytes in the buffer to 0
		memset(packetBuffer, 0, NTP_PACKET_SIZE);
		// Initialize values needed to form NTP request
		// (see URL above for details on the packets)
		packetBuffer[0] = 0b11100011;   // LI, Version, Mode
		packetBuffer[1] = 0;     // Stratum, or type of clock
		packetBuffer[2] = 6;     // Polling Interval
		packetBuffer[3] = 0xEC;  // Peer Clock Precision
		// 8 bytes of zero for Root Delay & Root Dispersion
		packetBuffer[12] = 49;
		packetBuffer[13] = 0x4E;
		packetBuffer[14] = 49;
		packetBuffer[15] = 52;
		// all NTP fields have been given values, now
		// you can send a packet requesting a timestamp:
		Udp.beginPacket(address, 123); //NTP requests are to port 123
		Udp.write(packetBuffer, NTP_PACKET_SIZE);
		Udp.endPacket();
	}

	void NTP::PrintDebug()
	{
	}
}
