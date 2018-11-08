#include "SensorData.h"
#include "SensorTransmissionResult.h"

namespace RX {
    #ifndef RFM69RXProxy_h
        #define RFM69RXProxy_h

        // library interface description
        class RFM69RXProxy {
            public:
                RFM69RXProxy();

                void Initialize();
                SensorTransmissionResult Listen();

            // library-accessible "private" interface
            private:
                void Blink(byte PIN, byte DELAY_MS, byte loops);
        };

    #endif
}
