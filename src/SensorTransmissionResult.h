#ifndef SensorTransmissionResult_h
    #define SensorTransmissionResult_h

    #include <Arduino.h>
    #include "Sensors\SensorData.h"

    using namespace Sensors;

    struct SensorTransmissionResult {
        SensorData Data;
        bool HasResult;
        uint8_t TransmissionSource;
        int16_t RSSI;

        void PrintDebug(){
            Serial.println(F("---------------------------------------"));
            Serial.print(F("Got packet from #")); Serial.print(TransmissionSource);
            Serial.print(F(" [RSSI :"));
            Serial.print(RSSI);
            Serial.println(F("]"));
            Serial.println(F("---------------------------------------"));
            Serial.println();
            
            Data.PrintDebug();
        }
    };

#endif
