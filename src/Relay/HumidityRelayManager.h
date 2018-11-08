#include "..\Sensors\SensorData.h"

namespace Relay {
    #ifndef HumidityRelayManager_h
        #define HumidityRelayManager_h

        // library interface description
        class HumidityRelayManager {
            public:
                HumidityRelayManager();

                void Initialize();
                void SetRelayState(SensorData *data);

            // library-accessible "private" interface
            private:
        };

    #endif
}
