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
                void AdjustClimate(SensorData *data);
                void KeepAlive();
                
            private:
                ControllerConfiguration *configuration;

                const int humidificationModeLedPin = LED_BUILTIN;
                const int dehumidificationModeLedPin = LED_BUILTIN;
                const int humidifierControlPin = 5;
                const int dehumidifierControlPin = 3;

                void EnableHumidifier();
                void DisableHumidifier();

                void EnableDehumidifier();
                void DisableDehumidifier();

                void ShutDown();
        };
    }

#endif
