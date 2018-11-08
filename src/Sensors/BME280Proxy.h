#include "SensorData.h"

namespace Sensors {
namespace BME280 {
    #ifndef BME280Proxy_h
        #define BME280Proxy_h

        // library interface description
        class BME280Proxy {
            public:
                enum TemperatureUnit {
                    C = 0x00,
                    F = 0x01
                };

                BME280Proxy(BME280Proxy::TemperatureUnit units = C);

                void Initialize();
                bool ReadSensor(SensorData *data);
                void PrintDebug();

            // library-accessible "private" interface
            private:
                void ConfigureBME280();
                TemperatureUnit units;
        };

    #endif
}
}
