#include <RH_RF69.h>
#include <RHReliableDatagram.h>
#include "RFM69TXProxy.h"
#include "TXResult.h"

using namespace Sensors;

namespace TX {
    RFM69TXProxy::RFM69TXProxy() : 
        rf69(this->RFM69_CS, this->RFM69_INT),
        rf69_manager(this->rf69, this->MY_ADDRESS)
    { }

    void RFM69TXProxy::Initialize() {
        pinMode(this->LED, OUTPUT);     
        pinMode(this->RFM69_RST, OUTPUT);
        digitalWrite(this->RFM69_RST, LOW);

        // manual reset
        digitalWrite(this->RFM69_RST, HIGH);
        delay(10);
        digitalWrite(this->RFM69_RST, LOW);
        delay(10);

        if (!this->rf69_manager.init()) {
            Serial.println(F("RFM69 radio init failed"));
            while (1);
        }
        
        // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
        // No encryption
        if (!this->rf69.setFrequency(RF69_FREQ)) {
            Serial.println(F("setFrequency failed"));
        }

        // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
        // ishighpowermodule flag set like this:
        this->rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

        // The encryption key has to be the same as the one in the server
        uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
        this->rf69.setEncryptionKey(key);

        pinMode(this->LED, OUTPUT);
    }

    // NOTE: for some reason, that I don't understand exactly, this needs to sit here rather than with the class.
    // dumb on me.
    uint8_t data[] = "  OK";

    TXResult RFM69TXProxy::Transmit(SensorData data) {
        TXResult result;
        result.TransmitSuccessful = false;

        // copy the sensor data into a buffer that can be used to transmit the data to the server.
        byte transmissionBuffer[sizeof(data)] = {0};
        memcpy(transmissionBuffer, &data, sizeof(data));
        byte dataSize = sizeof(data);

        // Send a message to the node designated as the server
        if (this->rf69_manager.sendtoWait((uint8_t *)transmissionBuffer, dataSize, this->SERVER_ADDRESS)) {
            // Now wait for a reply from the server
            uint8_t acknowledgementBufferLength = sizeof(this->acknowledgementBuffer);
            uint8_t from;

            if (this->rf69_manager.recvfromAckTimeout(this->acknowledgementBuffer, &acknowledgementBufferLength, 2000, &from)) {
                this->acknowledgementBuffer[acknowledgementBufferLength] = 0; // zero out (no idea why we do this, but it works!)

                Serial.print(F("Got reply from #"));
                Serial.print(from);
                Serial.print(F(" [RSSI :"));
                Serial.print(rf69.lastRssi());
                Serial.print(F("] : "));
                Serial.println((char*)this->acknowledgementBuffer);

                Blink(40, 3); //blink LED 3 times, 40ms between blinks

                result.From = from;
                result.RSSI = this->rf69.lastRssi();
                result.TransmitSuccessful = true;
            } else {
                Serial.println(F("No reply, is anyone listening?"));
            }
        } else {
            Serial.println(F("Sending failed (no ack)"));
        }

        return result;
    }

    void RFM69TXProxy::Blink(byte delay_ms, byte loops) {
        for (byte i = 0; i < loops; i++)  {
            digitalWrite(this->LED,HIGH);
            delay(delay_ms);
            digitalWrite(this->LED,LOW);
            delay(delay_ms);
        }
    }
}
