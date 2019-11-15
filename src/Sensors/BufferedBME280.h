#ifndef BufferedBME280_h
#define BufferedBME280_h

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
			temperatureBuffer.~CircularBuffer();
			humidityBuffer.~CircularBuffer();
		}

		void Add(BME280Data data)
		{
			temperatureBuffer.Add(data.Temperature);
			humidityBuffer.Add(data.Humidity);

			this->Temperature = temperatureBuffer.Average();
			this->Humidity = humidityBuffer.Average();
		}
	private:
		int arraySize = 0;

		CircularBuffer<float> temperatureBuffer;
		CircularBuffer<float> humidityBuffer;
	};
}
#endif
