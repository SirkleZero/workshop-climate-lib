namespace Relay {
    #ifndef HumidityRelayManager_h
        #define HumidityRelayManager_h

        #include "..\Sensors\SensorData.h"

        class HumidityRelayManager {
            public:
                HumidityRelayManager();

                void Initialize();
                void SetRelayState(SensorData *data);
                
            private:
                const int ledPin =  LED_BUILTIN; // use the built-in led
                const int relayControlPin = 5;
                const int switchPin = 9;

                int relayState = LOW;
        };

    #endif
}
