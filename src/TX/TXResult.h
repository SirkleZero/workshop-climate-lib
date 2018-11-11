#ifndef TXResult_h
    #define TXResult_h

    namespace TX {
        struct TXResult {
            bool TransmitSuccessful;
            uint8_t From;
            int16_t RSSI;

            void PrintDebug(){
                Serial.println(F("---------------------------------------"));
                Serial.print(F("TransmitSuccessful = ")); Serial.println(TransmitSuccessful);

                Serial.print(F("From = ")); Serial.println(From);

                Serial.print(F("RSSI = ")); Serial.println(RSSI);
                Serial.println(F("---------------------------------------"));
                Serial.println();
            }
        };
    }
#endif
