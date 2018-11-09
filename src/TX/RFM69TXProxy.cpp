#include <RH_RF69.h>
#include <RHReliableDatagram.h>
#include "RFM69TXProxy.h"
#include "TXResult.h"

using namespace Sensors;

namespace TX {
    /************ Radio Setup ***************/

    // Change to 434.0 or other frequency, must match RX's freq!
    #define RF69_FREQ       915.0

    // Where to send packets to!
    #define SERVER_ADDRESS    1
    // change addresses for each client board, any number :)
    #define MY_ADDRESS      2

    #define RFM69_CS        8
    #define RFM69_INT       3
    #define RFM69_RST       4
    #define LED             13

    // Singleton instance of the radio driver
    RH_RF69 rf69(RFM69_CS, RFM69_INT);

    // Class to manage message delivery and receipt, using the driver declared above
    RHReliableDatagram rf69_manager(rf69, MY_ADDRESS);

    // Constructor
    RFM69TXProxy::RFM69TXProxy() { }

    void RFM69TXProxy::Initialize() {
        pinMode(LED, OUTPUT);     
        pinMode(RFM69_RST, OUTPUT);
        digitalWrite(RFM69_RST, LOW);

        Serial.println("Feather Addressed RFM69 TX Test!");
        Serial.println();

        // manual reset
        digitalWrite(RFM69_RST, HIGH);
        delay(10);
        digitalWrite(RFM69_RST, LOW);
        delay(10);

        if (!rf69_manager.init()) {
            Serial.println("RFM69 radio init failed");
            while (1);
        }
        Serial.println("RFM69 radio init OK!");
        // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
        // No encryption
        if (!rf69.setFrequency(RF69_FREQ)) {
            Serial.println("setFrequency failed");
        }

        // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
        // ishighpowermodule flag set like this:
        rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

        // The encryption key has to be the same as the one in the server
        uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
        rf69.setEncryptionKey(key);

        pinMode(LED, OUTPUT);

        Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");
    }

    // Dont put this on the stack - it's reused; this is more efficient
    uint8_t acknowledgementBuffer[RH_RF69_MAX_MESSAGE_LEN]; // used for the response / ack?
    uint8_t data[] = "  OK";

    TXResult RFM69TXProxy::Transmit(SensorData data) {
        TXResult result;
        result.TransmitSuccessful = false;

        // copy the sensor data into a buffer that can be used to transmit the data to the server.
        byte transmissionBuffer[sizeof(data)] = {0};
        memcpy(transmissionBuffer, &data, sizeof(data));
        byte dataSize = sizeof(data);

        Serial.print("sending "); Serial.print(dataSize); Serial.println(" bytes of data");

        // Send a message to the node designated as the server
        if (rf69_manager.sendtoWait((uint8_t *)transmissionBuffer, dataSize, SERVER_ADDRESS)) {
            // Now wait for a reply from the server
            uint8_t acknowledgementBufferLength = sizeof(acknowledgementBuffer);
            uint8_t from;

            if (rf69_manager.recvfromAckTimeout(acknowledgementBuffer, &acknowledgementBufferLength, 2000, &from)) {
                acknowledgementBuffer[acknowledgementBufferLength] = 0; // zero out (no idea why we do this, but it works!)

                Serial.print("Got reply from #");
                Serial.print(from);
                Serial.print(" [RSSI :");
                Serial.print(rf69.lastRssi());
                Serial.print("] : ");
                Serial.println((char*)acknowledgementBuffer);

                Blink(LED, 40, 3); //blink LED 3 times, 40ms between blinks

                result.From = from;
                result.RSSI = rf69.lastRssi();
                result.TransmitSuccessful = true;
            } else {
                Serial.println("No reply, is anyone listening?");
            }
        } else {
            Serial.println("Sending failed (no ack)");
        }

        return result;
    }

    void RFM69TXProxy::Blink(byte PIN, byte DELAY_MS, byte loops) {
        for (byte i = 0; i < loops; i++)  {
            digitalWrite(PIN,HIGH);
            delay(DELAY_MS);
            digitalWrite(PIN,LOW);
            delay(DELAY_MS);
        }
    }
}
