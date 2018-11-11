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
                void AdjustClimate(SensorData data);
                void KeepAlive();
                void EmergencyShutoff();
                
            private:
                ControllerConfiguration *configuration;

                const int humidificationModeLedPin = LED_BUILTIN;
                const int dehumidificationModeLedPin = LED_BUILTIN;
                const int humidifierControlPin = 5;
                const int dehumidifierControlPin = 3;

                unsigned long currentMillis = 0; // stores the current value of millis()
                unsigned long previousKeepAliveCall = 0; // stores the last time KeepAlive() was called
                unsigned long previousEnableCall = 0; // stores the last time one of the relay's was turned on

                void EnableHumidifier();
                void DisableHumidifier();

                void EnableDehumidifier();
                void DisableDehumidifier();

                void ShutDown();                
        };
    }

#endif
