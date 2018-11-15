#ifndef ControllerConfiguration_h
    #define ControllerConfiguration_h

    #include <Arduino.h>

    namespace Configuration {
        class ControllerConfiguration {
            private:

            public:
                unsigned long RunawayTimeLimit;
                float MinimumHumidity;
                float TargetHumidity;
                float MaximumHumidity;
                float HumidityOffset;

                void PrintDebug(){
                    Serial.println(F("---------------------------------------"));
                    Serial.print(F("Runaway Time Limit = ")); Serial.println(this->RunawayTimeLimit);
                    Serial.print(F("Minimum Humidity = ")); Serial.println(this->MinimumHumidity);
                    Serial.print(F("Target Humidity = ")); Serial.println(this->TargetHumidity);
                    Serial.print(F("Maximum Humidity = ")); Serial.println(this->MaximumHumidity);
                    Serial.print(F("Humidity Offset = ")); Serial.println(this->HumidityOffset);
                    Serial.println(F("---------------------------------------"));
                    Serial.println();
                }
        };
    }
#endif
