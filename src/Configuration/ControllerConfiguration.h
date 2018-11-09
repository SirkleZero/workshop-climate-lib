#ifndef ControllerConfiguration_h
    #define ControllerConfiguration_h

    #include <Arduino.h>

    namespace Configuration {
        class ControllerConfiguration {
            private:

            public:
                unsigned long RunawayTimeLimit;
                float MinimumHumidity;
                float MaximumHumidity;
                float HumidityOffset;

                void PrintDebug(){
                    Serial.println("---------------------------------------");
                    Serial.print("Runaway Time Limit = "); Serial.println(this->RunawayTimeLimit);
                    Serial.print("Minimum Humidity = "); Serial.println(this->MinimumHumidity);
                    Serial.print("Maximum Humidity = "); Serial.println(this->MaximumHumidity);
                    Serial.print("Humidity Offset = "); Serial.println(this->HumidityOffset);
                    Serial.println("---------------------------------------");
                    Serial.println();
                }
        };
    }
#endif
