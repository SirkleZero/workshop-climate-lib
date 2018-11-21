#ifndef BME280Proxy_h
#define BME280Proxy_h

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "SensorData.h"

namespace Sensors {
	namespace BME280 {
		/// <summary></summary>
		class BME280Proxy {
		public:
			/// <summary></summary>
			const float SeaLevelPressure_hPa = 1013.25;
			/// <summary></summary>
			const float humidityOffset = 0;

			/// <summary></summary>
			enum TemperatureUnit {
				C = 0x00,
				F = 0x01
			};

			BME280Proxy(BME280Proxy::TemperatureUnit units = C);

			void Initialize();
			bool ReadSensor(SensorData *data);
			void PrintDebug();

		private:
			Adafruit_BME280 bme;
			TemperatureUnit units;
		};
	}
}
#endif
