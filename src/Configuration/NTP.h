#ifndef NTP_h
#define NTP_h

#include <Arduino.h>
#include <TimeLib.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <SPI.h>

#include "Secrets.h"
#include "..\InitializationResult.h"


namespace Configuration {
	/// <summary>Provides a class that wraps NTP communication for time synchronization.</summary>
	class NTP {
	public:
		NTP();

		InitializationResult Initialize(Secrets *secrets);
		void PrintDebug();
	private:
		Secrets *secrets;

		// NTP Servers:
		IPAddress timeServer;
		const char* ntpServerName = "0.pool.ntp.org";
		const int timeZone = -6; // central time

		WiFiUDP Udp;
		unsigned int localPort = 8888;  // local port to listen for UDP packets

		time_t getNtpTime();
		void sendNTPpacket(IPAddress &address);
	};
}
#endif
