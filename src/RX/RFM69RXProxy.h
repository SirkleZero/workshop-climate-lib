#ifndef RFM69RXProxy_h
    #define RFM69RXProxy_h
    
    #include <Arduino.h>
    #include <RH_RF69.h>
    #include <RHReliableDatagram.h>
    #include "..\Sensors\SensorData.h"
    #include "..\SensorTransmissionResult.h"

    using namespace Sensors;

    namespace RX {        
        class RFM69RXProxy {
            public:
                RFM69RXProxy();

                void Initialize();
                SensorTransmissionResult Listen();
            private:
                const int16_t RF69_FREQ = 915.0;
                const byte SERVER_ADDRESS = 1;
                const byte RFM69_CS = 12;
                const byte RFM69_RST = 11;
                const byte RFM69_IRQ = 13;
                //const byte RFM69_IRQN = digitalPinToInterrupt(RFM69_IRQ);

                RH_RF69 rf69;
                RHReliableDatagram manager;

                uint8_t messageBuffer[RH_RF69_MAX_MESSAGE_LEN];
        };
    }
#endif
