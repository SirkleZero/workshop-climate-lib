#pragma once

#include <Arduino.h>
#include "ParticleData.h"

namespace Sensors {
	/// <summary>Structure used to deserialize data from the PMS5003 sensor.</summary>
	struct PMS5003Frame {
		/// <summary>The header of the data frame.</summary>
		uint8_t FrameHeader[2];
		/// <summary>The length of the frame of data.</summary>
		uint16_t FrameLength = 0;
		/// <summary>The <see cref="ParticleData"/> that contains the actual sensor readings.</summary>
		ParticleData Particulates;
		/// <summary>The version, not entirely sure what this represents to be honest.</summary>
		uint8_t Version = 0;
		/// <summary>Unused</summary>
		uint8_t ErrorCode = 0;
		/// <summary>The checksum of the frame, used for error handling.</summary>
		uint16_t Checksum = 0;
	};
}
