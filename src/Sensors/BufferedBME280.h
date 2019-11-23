#pragma once

#include <Arduino.h>
#include "BME280Data.h"
#include "..\CircularBuffer.h"

namespace Sensors {
	/// <summary>Represents data related to the climate that has been buffered over a period of time.</summary>
	struct BufferedBME280 : BME280Data
	{
	public:
		BufferedBME280(int size = 10) :
			temperatureBuffer(size),
			humidityBuffer(size)
		{
			this->arraySize = size;
		}

		~BufferedBME280()
		{
			this->temperatureBuffer.~CircularBuffer();
			this->humidityBuffer.~CircularBuffer();
		}

		void Add(BME280Data data)
		{
			this->temperatureBuffer.Append(data.Temperature);
			this->humidityBuffer.Append(data.Humidity);

			this->Temperature = this->temperatureBuffer.Average();
			this->Humidity = this->humidityBuffer.Average();
		}
	private:
		int arraySize = 0;

		CircularBuffer<float> temperatureBuffer;
		CircularBuffer<float> humidityBuffer;
	};
}
