#ifndef RFM69TXProxy_h
    #define RFM69TXProxy_h

    #include "..\Sensors\SensorData.h"
    #include "TXResult.h"

    using namespace Sensors;

    namespace TX {
        class RFM69TXProxy {
            public:
                RFM69TXProxy();

                void Initialize();
                TXResult Transmit(SensorData data);
                void PrintDebug();
                
            private:
                const int16_t RF69_FREQ = 915.0;
                const byte SERVER_ADDRESS = 1;
                const byte MY_ADDRESS = 2;
                const byte RFM69_CS = 8;
                const byte RFM69_INT = 3;
                const byte RFM69_RST = 4;
                const byte LED = 13;

                RH_RF69 rf69;
                RHReliableDatagram rf69_manager;

                uint8_t acknowledgementBuffer[RH_RF69_MAX_MESSAGE_LEN];

                void Blink(byte delay_ms, byte loops);
        };
    }
#endif
