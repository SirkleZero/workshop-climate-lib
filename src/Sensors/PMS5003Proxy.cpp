#include "PMS5003Proxy.h"

namespace Sensors {
	namespace PMS5003 {
		/// <summary>Initializes a new instance of the <see cref="PMS5003Proxy"/> class.</summary>
		PMS5003Proxy::PMS5003Proxy() {}

		/// <summary>Executes initialization logic for the object.</summary>
		void PMS5003Proxy::Initialize()
		{
			thisFrame.FrameLength = PMS5003MaxFrameLength;
		}

		/// <summary>Reads data from the sensor.</summary>
		/// <param name="data">The <see cref="SensorData"/> object to place the data into.</param>
		bool PMS5003Proxy::ReadSensor(SensorData *data)
		{
			// send data only when you receive data. This sensor communicates over the Serial1 interface based on how we have it hooked up to the microcontroller.
			Serial1.begin(9600);
			bool packetReceived = false;

			while (!packetReceived)
			{
				if (Serial1.available() > 32)
				{
					int drain = Serial1.available();
					if (InDebugMode)
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
					if (InDebugMode)
					{
						Serial.print(F("-- Available: "));
						Serial.println(Serial1.available(), DEC);
					}
					incomingByte = Serial1.read();
					if (InDebugMode)
					{
						Serial.print(F("-- READ: "));
						Serial.println(incomingByte, HEX);
					}
					if (!inFrame)
					{
						if (incomingByte == 0x42 && detectOff == 0)
						{
							frameBuffer[detectOff] = incomingByte;
							thisFrame.FrameHeader[0] = incomingByte;
							calculatedChecksum = incomingByte; // Checksum init!
							detectOff++;
						}
						else if (incomingByte == 0x4D && detectOff == 1)
						{
							frameBuffer[detectOff] = incomingByte;
							thisFrame.FrameHeader[1] = incomingByte;
							calculatedChecksum += incomingByte;
							inFrame = true;
							detectOff++;
						}
						else
						{
							Serial.print(F("-- Frame syncing... "));
							Serial.print(incomingByte, HEX);
							if (InDebugMode)
							{
							}
							Serial.println();
						}
					}
					else
					{
						frameBuffer[detectOff] = incomingByte;
						calculatedChecksum += incomingByte;
						detectOff++;
						uint16_t val = frameBuffer[detectOff - 1] + (frameBuffer[detectOff - 2] << 8);
						switch (detectOff)
						{
							case 4:
								thisFrame.FrameLength = val;
								frameLength = val + detectOff;
								break;
							case 6:
								thisFrame.Particulates.pm10_standard = val;
								break;
							case 8:
								thisFrame.Particulates.pm25_standard = val;
								break;
							case 10:
								thisFrame.Particulates.pm100_standard = val;
								break;
							case 12:
								thisFrame.Particulates.pm10_env = val;
								break;
							case 14:
								thisFrame.Particulates.pm25_env = val;
								break;
							case 16:
								thisFrame.Particulates.pm100_env = val;
								break;
							case 18:
								thisFrame.Particulates.particles_03um = val;
								break;
							case 20:
								thisFrame.Particulates.particles_05um = val;
								break;
							case 22:
								thisFrame.Particulates.particles_10um = val;
								break;
							case 24:
								thisFrame.Particulates.particles_25um = val;
								break;
							case 26:
								thisFrame.Particulates.particles_50um = val;
								break;
							case 28:
								thisFrame.Particulates.particles_100um = val;
								break;
							case 29:
								val = frameBuffer[detectOff - 1];
								thisFrame.Version = val;
								break;
							case 30:
								val = frameBuffer[detectOff - 1];
								thisFrame.ErrorCode = val;
								break;
							case 32:
								thisFrame.Checksum = val;
								calculatedChecksum -= ((val >> 8) + (val & 0xFF));
								break;
							default:
								break;
						}

						if (detectOff >= frameLength)
						{
							packetReceived = true;
							detectOff = 0;
							inFrame = false;
						}
					}
				}
			}

			Serial1.end();

			if (calculatedChecksum == thisFrame.Checksum)
			{
				data->particulates = thisFrame.Particulates;
				return true;
			}

			return false;
		}
	}
}
