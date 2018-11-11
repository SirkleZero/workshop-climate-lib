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
                Serial.print(F("Is Success: ")); Serial.println(this->IsSuccess);
                Serial.print(F("SSID: ")); Serial.println(this->SSID);
                Serial.print(F("RSSI: ")); Serial.println(this->RSSI);
                Serial.print(F("Local IP: ")); Serial.println(this->LocalIP);
                Serial.print(F("Gateway IP: ")); Serial.println(this->GatewayIP);
                Serial.print(F("Subnet Mask: ")); Serial.println(this->SubnetMask);
                Serial.print(F("Error Message: ")); Serial.println(this->ErrorMessage);
                Serial.println(F("---------------------------------------"));
                Serial.println();
            }
        };
    }
#endif
