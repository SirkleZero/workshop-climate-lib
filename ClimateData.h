#include <Arduino.h>
#include <avr/dtostrf.h>

#ifndef ClimateData_h
    #define ClimateData_h

    struct ClimateData {
        float Temperature;   // [deg C]
        char Units;
        float Humidity;      // [%]
        float Pressure;      // pascals

        void PrintDebug(){
            Serial.println("---------------------------------------");
            Serial.print("Temperature = ");
            Serial.print(Temperature);
            Serial.print(" *");
            Serial.println(Units);

            Serial.print("Pressure = ");
            Serial.print(Pressure / 100.0F);
            Serial.println(" hPa"); // pressure in pascals

            Serial.print("Humidity = ");
            Serial.print(Humidity);
            Serial.println(" %");
            Serial.println("---------------------------------------");
            Serial.println();
        }
        
        static float ConvertCToF(float c) {
            return (c * 9/5) + 32;
        }

        static char* ConvertFloatToString(float f, signed char width, unsigned char precision) {
            char buff[10];
            dtostrf(f, width, precision, buff);
            return buff;
        }
    };

#endif
