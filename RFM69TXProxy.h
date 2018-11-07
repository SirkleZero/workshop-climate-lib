#include "SensorData.h"
#include "TXResult.h"

namespace TX {
    #ifndef RFM69TXProxy_h
        #define RFM69TXProxy_h

        // library interface description
        class RFM69TXProxy {
            public:
                RFM69TXProxy();

                void Initialize();
                TXResult Transmit(SensorData data);
                void PrintDebug();

            // library-accessible "private" interface
            private:
                void Blink(byte PIN, byte DELAY_MS, byte loops);
        };

    #endif
}
