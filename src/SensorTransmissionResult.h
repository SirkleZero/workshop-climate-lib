#ifndef SensorTransmissionResult_h
    #define SensorTransmissionResult_h

    #include "Sensors\SensorData.h"

    using namespace Sensors;

    struct SensorTransmissionResult {
        SensorData Data;
        bool HasResult;
        uint8_t TransmissionSource;
        int16_t RSSI;

        void PrintDebug(){
            Serial.println("---------------------------------------");
            Serial.print("Got packet from #"); Serial.print(TransmissionSource);
            Serial.print(" [RSSI :");
            Serial.print(RSSI);
            Serial.println("]");
            Serial.println("---------------------------------------");
            Serial.println();
            
            Data.PrintDebug();
        }
    };

#endif
