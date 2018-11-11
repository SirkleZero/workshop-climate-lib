#ifndef HumidityRelayManager_h
    #define HumidityRelayManager_h

    #include <Arduino.h>
    
    #include "..\Sensors\SensorData.h"
    #include "..\Configuration\ControllerConfiguration.h"
    
    using namespace Configuration;
    using namespace Sensors;

    namespace Relay {
        class HumidityRelayManager {
            public:
                HumidityRelayManager();

                void Initialize(ControllerConfiguration *configuration);
                void SetRelayState(SensorData *data);
                void KeepAlive();
                
            private:
                ControllerConfiguration *configuration;

                const int humidificationModeLedPin = LED_BUILTIN;
                const int dehumidificationModeLedPin = LED_BUILTIN;
                const int humidifierControlPin = 5;
                const int dehumidifierControlPin = 3;

                byte relayState = LOW;
        };
    }

#endif
