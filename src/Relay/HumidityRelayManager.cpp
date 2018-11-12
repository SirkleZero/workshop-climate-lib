#include "HumidityRelayManager.h"

using namespace Sensors;

namespace Relay {    
    HumidityRelayManager::HumidityRelayManager() { }

    void HumidityRelayManager::Initialize(ControllerConfiguration *configuration) {
        this->configuration = configuration;

        // relay control pins
        pinMode(HumidityRelayManager::HumidifierControlPin, OUTPUT);
        pinMode(HumidityRelayManager::DehumidifierControlPin, OUTPUT);
        
        this->EnableIndicator();

        this->SetIndicatorColor(0xe6, 0x5c, 0x00); // orange
    }

    void HumidityRelayManager::KeepAlive() {
        this->previousKeepAliveCall = millis();
    }

    void HumidityRelayManager::AdjustClimate(SensorData data) {
        this->KeepAlive();

        // figure out which mode we need to be in. Humidification or Dehumidification.
        if(data.climate.Humidity > this->configuration->MaximumHumidity){
            // it's too humid, enable the dehumidifier
            this->DisableHumidifier();
            this->EnableDehumidifier();
        } else if (data.climate.Humidity < this->configuration->MinimumHumidity){
            // it's too dry, enable the humidifier
            this->DisableDehumidifier();
            this->EnableHumidifier();
        } else {
            // goldilocks zone, shut them both down
            this->ShutDown();
            this->SetIndicatorColor(0x33, 0xcc, 0x33); // green
        }
    }

    void HumidityRelayManager::EmergencyShutoff() {
        currentMillis = millis();
        if(this->currentMillis - this->previousKeepAliveCall >= this->configuration->RunawayTimeLimit) {
            // we exceeded our time! Shut it down!
            Serial.println(F("HumidityRelayManager: runaway limit reached!"));
            this->ShutDown();
            this->SetIndicatorColor(0xe6, 0x5c, 0x00); // orange
        }
    }

    void HumidityRelayManager::EnableHumidifier() {
        //Serial.println(F("HumidityRelayManager: Enabling the humidifier"));
        this->SetIndicatorColor(0x00, 0x99, 0xff);  // aqua
        digitalWrite(HumidityRelayManager::HumidifierControlPin, HIGH);
    }

    void HumidityRelayManager::DisableHumidifier() {
        //Serial.println(F("HumidityRelayManager: Disabling the humidifier"));
        digitalWrite(HumidityRelayManager::HumidifierControlPin, LOW);
    }

    void HumidityRelayManager::EnableDehumidifier() {
        //Serial.println(F("HumidityRelayManager: Enabling the dehumidifier"));
        this->SetIndicatorColor(0x99, 0x00, 0x00);  // red
        digitalWrite(HumidityRelayManager::DehumidifierControlPin, HIGH);
    }

    void HumidityRelayManager::DisableDehumidifier() {
        //Serial.println(F("HumidityRelayManager: Disabling the dehumidifier"));
        digitalWrite(HumidityRelayManager::DehumidifierControlPin, LOW);
    }

    void HumidityRelayManager::ShutDown() {
        //Serial.println(F("HumidityRelayManager: Goldilocks! Shutting down humidifier AND dehumidifier!"));
        this->DisableHumidifier();
        this->DisableDehumidifier();
    }

    void HumidityRelayManager::SetIndicatorColor(int red, int green, int blue) {
        #ifdef COMMON_ANODE
            red = 255 - red;
            green = 255 - green;
            blue = 255 - blue;
        #endif

        this->EnableIndicator();

        analogWrite(HumidityRelayManager::RedPin, red);
        analogWrite(HumidityRelayManager::GreenPin, green);
        analogWrite(HumidityRelayManager::BluePin, blue);
    }

    void HumidityRelayManager::DisableIndicator() {
        if(this->indicatorEnabled) {
            pinMode(HumidityRelayManager::RedPin, INPUT);
            pinMode(HumidityRelayManager::GreenPin, INPUT);
            pinMode(HumidityRelayManager::BluePin, INPUT);

            this->indicatorEnabled = false;
        }
    }

    void HumidityRelayManager::EnableIndicator() {
        if(!this->indicatorEnabled) {
            pinMode(HumidityRelayManager::RedPin, OUTPUT);
            pinMode(HumidityRelayManager::GreenPin, OUTPUT);
            pinMode(HumidityRelayManager::BluePin, OUTPUT);

            this->indicatorEnabled = true;
        }
    }
}
