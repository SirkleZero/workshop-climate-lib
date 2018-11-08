#include <SPI.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>
#include "RFM69RXProxy.h"

namespace RX {
    /************ Radio Setup ***************/
    // Change to 434.0 or other frequency, must match RX's freq!
    #define RF69_FREQ 915.0

    // who am i? (server address)
    #define SERVER_ADDRESS     1
    
    #define RFM69_CS      12
    #define RFM69_RST     11
    #define RFM69_IRQ     13
    #define RFM69_IRQN    digitalPinToInterrupt(RFM69_IRQ)
    //#define LED           13

    // Singleton instance of the radio driver
    // TODO: What happens if we pass the RFM69_IRQN value rather than the RFM69_IRQ value to the below function?
    // would this mean we wouldn't have to re-initialize and/or shut down the wifi every time we hit our loop in
    // the sketch?!
    RH_RF69 rf69(RFM69_CS, RFM69_IRQ);

    // Class to manage message delivery and receipt, using the driver declared above
    RHReliableDatagram manager(rf69, SERVER_ADDRESS);

    // Constructor
    RFM69RXProxy::RFM69RXProxy() { }

    void RFM69RXProxy::Initialize() {
        //pinMode(LED, OUTPUT);     
        pinMode(RFM69_RST, OUTPUT);
        digitalWrite(RFM69_RST, LOW);

        Serial.println("Feather Addressed RFM69 RX Test!");

        // manual reset
        digitalWrite(RFM69_RST, HIGH);
        delay(10);
        digitalWrite(RFM69_RST, LOW);
        delay(10);
        
        if (!manager.init()) {
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
        
        //pinMode(LED, OUTPUT);

        Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");
    }

    // Dont put these on the stack, they are re-used; this is more efficient.
    uint8_t acknowledgeData[] = "acknowledged from node 1";
    uint8_t messageBuffer[RH_RF69_MAX_MESSAGE_LEN];

    SensorTransmissionResult RFM69RXProxy::Listen() {
        SensorTransmissionResult result;
        result.HasResult = false;

        if (manager.available()) {
            // Wait for a message addressed to us from the client
            uint8_t messageBufferLength = sizeof(messageBuffer);
            uint8_t from;

            if (manager.recvfromAck(messageBuffer, &messageBufferLength, &from)) {
                messageBuffer[messageBufferLength] = 0; // zero out (I don't really know why we do this to be honest)

                // deserialize the message sent into our common data structure
                SensorData messageData;
                memcpy(&messageData, messageBuffer, sizeof(messageData));
                result.Data = messageData;
                result.TransmissionSource = from;
                result.RSSI = rf69.lastRssi();
                result.HasResult = true;
                
                //blink LED 3 times, 40ms between blinks
                //Blink(LED, 40, 3);

                // Send a reply back to the originator client
                if (!manager.sendtoWait(acknowledgeData, sizeof(acknowledgeData), from)) {
                    Serial.println("Sending failed (no ack)");
                }
            }
        }

        return result;
    }
    
    // void RFM69RXProxy::Blink(byte PIN, byte DELAY_MS, byte loops) {
    //     for (byte i = 0; i < loops; i++)  {
    //         digitalWrite(PIN,HIGH);
    //         delay(DELAY_MS);
    //         digitalWrite(PIN,LOW);
    //         delay(DELAY_MS);
    //     }
    // }
}
