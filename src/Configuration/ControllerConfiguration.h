#ifndef ControllerConfiguration_h
    #define ControllerConfiguration_h

    #include <Arduino.h>

    namespace Configuration {
        class ControllerConfiguration {
            private:

            public:
                unsigned long SampleFrequency;
                float MinimumHumidity;
                float MaximumHumidity;
                float HumidityOffset;

                void PrintDebug(){
                    Serial.println("---------------------------------------");
                    Serial.print(F("Sample Frequency = ")); Serial.println(this->SampleFrequency);
                    Serial.print(F("Minimum Humidity = ")); Serial.println(this->MinimumHumidity);
                    Serial.print(F("Maximum Humidity = ")); Serial.println(this->MaximumHumidity);
                    Serial.print(F("Humidity Offset = ")); Serial.println(this->HumidityOffset);
                    Serial.println("---------------------------------------");
                    Serial.println();
                }
        };
    }
#endif
