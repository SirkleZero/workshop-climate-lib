#include "PMS5003Proxy.h"

namespace Sensors {
	namespace PMS5003 {
		const bool DEBUG = false;
		const int MAX_FRAME_LEN = 64;

		int incomingByte = 0; // for incoming serial data
		char frameBuf[MAX_FRAME_LEN];
		int detectOff = 0;
		int frameLen = MAX_FRAME_LEN;
		bool inFrame = false;
		char printbuf[256];
		uint16_t calcChecksum = 0;

		PMS5003Frame thisFrame;

		// Constructor
		PMS5003Proxy::PMS5003Proxy() {}

		void PMS5003Proxy::Initialize()
		{
			// configure the PMS5003 sensor
			ConfigurePMS5003();
		}

		void PMS5003Proxy::ConfigurePMS5003()
		{
			thisFrame.frameLen = MAX_FRAME_LEN;
		}

		bool PMS5003Proxy::ReadSensor(SensorData *data)
		{
			// send data only when you receive data:
			Serial1.begin(9600);
			bool packetReceived = false;

			while (!packetReceived)
			{
				if (Serial1.available() > 32)
				{
					int drain = Serial1.available();
					if (DEBUG)
					{
						Serial.print(F("-- Draining buffer: "));
						Serial.println(Serial1.available(), DEC);
					}
					for (int i = drain; i > 0; i--)
					{
						Serial1.read();
					}
				}
				if (Serial1.available() > 0)
				{
					if (DEBUG)
					{
						Serial.print(F("-- Available: "));
						Serial.println(Serial1.available(), DEC);
					}
					incomingByte = Serial1.read();
					if (DEBUG)
					{
						Serial.print(F("-- READ: "));
						Serial.println(incomingByte, HEX);
					}
					if (!inFrame)
					{
						if (incomingByte == 0x42 && detectOff == 0)
						{
							frameBuf[detectOff] = incomingByte;
							thisFrame.frameHeader[0] = incomingByte;
							calcChecksum = incomingByte; // Checksum init!
							detectOff++;
						}
						else if (incomingByte == 0x4D && detectOff == 1)
						{
							frameBuf[detectOff] = incomingByte;
							thisFrame.frameHeader[1] = incomingByte;
							calcChecksum += incomingByte;
							inFrame = true;
							detectOff++;
						}
						else
						{
							Serial.print(F("-- Frame syncing... "));
							Serial.print(incomingByte, HEX);
							if (DEBUG)
							{
							}
							Serial.println();
						}
					}
					else
					{
						frameBuf[detectOff] = incomingByte;
						calcChecksum += incomingByte;
						detectOff++;
						uint16_t val = frameBuf[detectOff - 1] + (frameBuf[detectOff - 2] << 8);
						switch (detectOff)
						{
							case 4:
								thisFrame.frameLen = val;
								frameLen = val + detectOff;
								break;
							case 6:
								thisFrame.particulates.pm10_standard = val;
								break;
							case 8:
								thisFrame.particulates.pm25_standard = val;
								break;
							case 10:
								thisFrame.particulates.pm100_standard = val;
								break;
							case 12:
								thisFrame.particulates.pm10_env = val;
								break;
							case 14:
								thisFrame.particulates.pm25_env = val;
								break;
							case 16:
								thisFrame.particulates.pm100_env = val;
								break;
							case 18:
								thisFrame.particulates.particles_03um = val;
								break;
							case 20:
								thisFrame.particulates.particles_05um = val;
								break;
							case 22:
								thisFrame.particulates.particles_10um = val;
								break;
							case 24:
								thisFrame.particulates.particles_25um = val;
								break;
							case 26:
								thisFrame.particulates.particles_50um = val;
								break;
							case 28:
								thisFrame.particulates.particles_100um = val;
								break;
							case 29:
								val = frameBuf[detectOff - 1];
								thisFrame.version = val;
								break;
							case 30:
								val = frameBuf[detectOff - 1];
								thisFrame.errorCode = val;
								break;
							case 32:
								thisFrame.checksum = val;
								calcChecksum -= ((val >> 8) + (val & 0xFF));
								break;
							default:
								break;
						}

						if (detectOff >= frameLen)
						{
							packetReceived = true;
							detectOff = 0;
							inFrame = false;
						}
					}
				}
			}

			Serial1.end();

			if (calcChecksum == thisFrame.checksum)
			{
				data->particulates = thisFrame.particulates;
				return true;
			}

			return false;
		}

		void PMS5003Proxy::PrintDebug()
		{
			SensorData data;
			this->ReadSensor(&data);

			Serial.println();
			Serial.println(F("---------------------------------------"));
			Serial.println(F("Concentration Units (standard)"));
			Serial.print(F("PM 1.0: ")); Serial.print(data.particulates.pm10_standard);
			Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.particulates.pm25_standard);
			Serial.print(F("\t\tPM 10: ")); Serial.println(data.particulates.pm100_standard);
			Serial.println(F("---------------------------------------"));
			Serial.println(F("Concentration Units (environmental)"));
			Serial.print(F("PM 1.0: ")); Serial.print(data.particulates.pm10_env);
			Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.particulates.pm25_env);
			Serial.print(F("\t\tPM 10: ")); Serial.println(data.particulates.pm100_env);
			Serial.println(F("---------------------------------------"));
			Serial.print(F("Particles > 0.3um / 0.1L air:")); Serial.println(data.particulates.particles_03um);
			Serial.print(F("Particles > 0.5um / 0.1L air:")); Serial.println(data.particulates.particles_05um);
			Serial.print(F("Particles > 1.0um / 0.1L air:")); Serial.println(data.particulates.particles_10um);
			Serial.print(F("Particles > 2.5um / 0.1L air:")); Serial.println(data.particulates.particles_25um);
			Serial.print(F("Particles > 5.0um / 0.1L air:")); Serial.println(data.particulates.particles_50um);
			Serial.print(F("Particles > 10.0 um / 0.1L air:")); Serial.println(data.particulates.particles_100um);
			Serial.println(F("---------------------------------------"));
		}
	}
}