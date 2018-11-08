namespace Configuration {
    #ifndef ControllerConfiguration_h
        #define ControllerConfiguration_h

        class ControllerConfiguration {
            private:

            public:
                unsigned long SampleFrequency;

                void PrintDebug(){
                    Serial.println("---------------------------------------");
                    Serial.print("Sample Frequency = "); Serial.println(this->SampleFrequency);
                    Serial.println("---------------------------------------");
                    Serial.println();
                }
        };

    #endif
}
