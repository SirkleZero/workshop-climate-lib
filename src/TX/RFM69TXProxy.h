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
                void Blink(byte PIN, byte DELAY_MS, byte loops);
        };
    }
#endif
