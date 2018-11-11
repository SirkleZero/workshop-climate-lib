#ifndef IoTUploadResult_h
    #define IoTUploadResult_h

    #include <WiFi101.h>

    namespace TX {
        struct IoTUploadResult {
            bool IsSuccess = false;

            char* SSID;
            int16_t RSSI;
            IPAddress LocalIP;
            IPAddress GatewayIP;
            IPAddress SubnetMask;
            const __FlashStringHelper *ErrorMessage;

            void PrintDebug(){
                Serial.println(F("---------------------------------------"));

                Serial.println(F("---------------------------------------"));
                Serial.println();
            }
        };
    }
#endif
