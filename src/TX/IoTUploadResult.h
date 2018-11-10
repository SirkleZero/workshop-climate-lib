#ifndef IoTUploadResult_h
    #define IoTUploadResult_h

    namespace TX {
        struct IoTUploadResult {
            bool IsSuccess = false;

            void PrintDebug(){
                Serial.println(F("---------------------------------------"));

                Serial.println(F("---------------------------------------"));
                Serial.println();
            }
        };
    }
#endif
