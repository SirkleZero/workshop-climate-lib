#ifndef PMS5003Proxy_h
#define PMS5003Proxy_h

#include <Arduino.h>

#include "..\InitializationResult.h"
#include "PMS5003Frame.h"
#include "ParticleData.h"

namespace Sensors {
	/// <summary>Describes the maximum frame size of data from the sensor.</summary>
	static const int PMS5003MaxFrameLength = 64;

	/// <summary>A class that handles the communication with the PMS5003 sensor.</summary>
	class PMS5003Proxy {
	public:
		PMS5003Proxy();

		InitializationResult Initialize();
		bool ReadSensor(ParticleData *data);
	private:
		/// <summary>true if we are debugging; otherwise false.</summary>
		const bool InDebugMode = false;

		char frameBuffer[PMS5003MaxFrameLength];
		int incomingByte = 0;
		int detectOff = 0;
		int frameLength = PMS5003MaxFrameLength;
		bool inFrame = false;
		char printBuffer[256];
		uint16_t calculatedChecksum = 0;

		PMS5003Frame thisFrame;
	};
}
#endif
