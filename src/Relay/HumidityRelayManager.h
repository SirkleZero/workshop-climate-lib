#ifndef HumidityRelayManager_h
    #define HumidityRelayManager_h

    #include <Arduino.h>
    
    #include "..\Sensors\SensorData.h"
    #include "..\Configuration\ControllerConfiguration.h"
    #include "..\Display\RGB.h"
    
    using namespace Configuration;
    using namespace Display;
    using namespace Sensors;

    namespace Relay {
		enum HumidificationState {
			Dehumidifying = 0x00,
			Humidifying = 0x01,
			None = 0x02
		};

        class HumidityRelayManager {
            public:
                HumidityRelayManager();

                void Initialize(ControllerConfiguration *configuration);
                void AdjustClimate(SensorData data);
                void KeepAlive();
                void EmergencyShutoff();
                
            private:
                // pins
                const int RedPin = 15;
                const int GreenPin = 16;
                const int BluePin = 17;
                const int HumidifierControlPin = 18;
                const int DehumidifierControlPin = 19;

                // if using a common anode LED, set this to true
                const bool CommonAnode = true;

                // colors
                const RGB Orange;
                const RGB Green;
                const RGB Purple;
                const RGB Aqua;
                const RGB Red;

                ControllerConfiguration *configuration;
                bool indicatorEnabled = false;
				HumidificationState humidificationState = HumidificationState::None;

                unsigned long currentMillis = 0; // stores the current value of millis()
                unsigned long previousKeepAliveCall = 0; // stores the last time KeepAlive() was called

                void EnableHumidifier();
                void DisableHumidifier();

                void EnableDehumidifier();
                void DisableDehumidifier();

                void ShutDown();
                void SetIndicatorColor(RGB color);
                void DisableIndicator();
                void EnableIndicator();     
        };
    }

#endif
