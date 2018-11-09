#ifndef ControllerConfiguration_h
    #define ControllerConfiguration_h

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
                    Serial.print("Sample Frequency = "); Serial.println(this->SampleFrequency);
                    Serial.print("Minimum Humidity = "); Serial.println(this->MinimumHumidity);
                    Serial.print("Maximum Humidity = "); Serial.println(this->MaximumHumidity);
                    Serial.print("Humidity Offset = "); Serial.println(this->HumidityOffset);
                    Serial.println("---------------------------------------");
                    Serial.println();
                }
        };
    }
#endif
