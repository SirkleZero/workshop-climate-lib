#include "SensorData.h"

namespace Sensors {
namespace PMS5003 {
    #ifndef PMS5003Proxy_h
        #define PMS5003Proxy_h

        // library interface description
        class PMS5003Proxy {
            public:
                PMS5003Proxy();

                void Initialize();
                bool ReadSensor(SensorData *data);
                void PrintDebug();

            // library-accessible "private" interface
            private:
                void ConfigurePMS5003();
        };

    #endif
}
}
