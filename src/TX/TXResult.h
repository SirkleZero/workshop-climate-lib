#ifndef TXResult_h
    #define TXResult_h

    namespace TX {
        struct TXResult {
            bool TransmitSuccessful;
            uint8_t From;
            int16_t RSSI;

            void PrintDebug(){
                Serial.println("---------------------------------------");
                Serial.print("TransmitSuccessful = ");
                Serial.println(TransmitSuccessful);

                Serial.print("From = ");
                Serial.println(From);

                Serial.print("RSSI = ");
                Serial.println(RSSI);
                Serial.println("---------------------------------------");
                Serial.println();
            }
        };
    }
#endif
