namespace Configuration {
    #ifndef ControllerConfiguration_h
        #define ControllerConfiguration_h

        class ControllerConfiguration {
            private:

            public:
                unsigned long SampleFrequency;
                byte MinimumHumidity;
                byte MaximumHumidity;
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

    #endif
}
