#ifndef PMS5003Frame_h
#define PMS5003Frame_h

#include <Arduino.h>
#include "ParticleData.h"

namespace Sensors {
	/// <summary></summary>
	struct PMS5003Frame {
		/// <summary></summary>
		uint8_t  frameHeader[2];
		/// <summary></summary>
		uint16_t frameLen;
		/// <summary></summary>
		ParticleData particulates;
		/// <summary></summary>
		uint8_t  version;
		/// <summary></summary>
		uint8_t  errorCode;
		/// <summary></summary>
		uint16_t checksum;
	};
}
#endif
