#ifndef HumidityRelayManager_h
    #define HumidityRelayManager_h

    #include <Arduino.h>
    
    #include "..\Sensors\SensorData.h"
    #include "..\Configuration\ControllerConfiguration.h"
    
    using namespace Configuration;
    using namespace Sensors;

    namespace Relay {
        class HumidityRelayManager {
            //uncomment this line if using a Common Anode LED
            #define COMMON_ANODE

            public:
                HumidityRelayManager();

                void Initialize(ControllerConfiguration *configuration);
                void AdjustClimate(SensorData data);
                void KeepAlive();
                void EmergencyShutoff();
                
            private:
                const int RedPin = 15;
                const int GreenPin = 16;
                const int BluePin = 17;
                const int HumidifierControlPin = 5;
                const int DehumidifierControlPin = 3;

                ControllerConfiguration *configuration;

                unsigned long currentMillis = 0; // stores the current value of millis()
                unsigned long previousKeepAliveCall = 0; // stores the last time KeepAlive() was called

                void EnableHumidifier();
                void DisableHumidifier();

                void EnableDehumidifier();
                void DisableDehumidifier();

                void ShutDown();
                void SetIndicatorColor(int red, int green, int blue);           
        };
    }

#endif
