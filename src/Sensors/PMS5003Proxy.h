#ifndef PMS5003Proxy_h
#define PMS5003Proxy_h

#include <Arduino.h>

#include "PMS5003Frame.h"
#include "SensorData.h"

namespace Sensors {
	namespace PMS5003 {
		class PMS5003Proxy {
		public:
			PMS5003Proxy();

			void Initialize();
			bool ReadSensor(SensorData *data);
			void PrintDebug();

		private:
			void ConfigurePMS5003();
		};
	}
}
#endif
