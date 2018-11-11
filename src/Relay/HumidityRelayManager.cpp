#include "HumidityRelayManager.h"

using namespace Sensors;

namespace Relay {    
    HumidityRelayManager::HumidityRelayManager() { }

    void HumidityRelayManager::Initialize(ControllerConfiguration *configuration) {
        this->configuration = configuration;

        // relay control pins
        pinMode(HumidityRelayManager::HumidifierControlPin, OUTPUT);
        pinMode(HumidityRelayManager::DehumidifierControlPin, OUTPUT);

        // relay indicator pins
        pinMode(HumidityRelayManager::RedPin, OUTPUT);
        pinMode(HumidityRelayManager::GreenPin, OUTPUT);
        pinMode(HumidityRelayManager::BluePin, OUTPUT);

        this->SetIndicatorColor(0, 255, 0);
    }

    void HumidityRelayManager::KeepAlive() {
        this->previousKeepAliveCall = millis();
    }

    void HumidityRelayManager::AdjustClimate(SensorData data) {
        this->KeepAlive();

        // figure out which mode we need to be in. Humidification or Dehumidification.
        if(data.climate.Humidity > this->configuration->MaximumHumidity){
            // it's too humid, enable the dehumidifier
            this->EnableDehumidifier();
        } else if (data.climate.Humidity < this->configuration->MinimumHumidity){
            // it's too dry, enable the humidifier
            this->EnableHumidifier();
        } else {
            // goldilocks zone, shut them both down
            this->ShutDown();
        }
    }

    void HumidityRelayManager::EmergencyShutoff() {
        currentMillis = millis();
        if(this->currentMillis - this->previousKeepAliveCall >= this->configuration->RunawayTimeLimit) {
            // we exceeded our time! Shut it down!
            Serial.println(F("HumidityRelayManager: runaway limit reached!"));
            this->ShutDown();
        }
    }

    void HumidityRelayManager::EnableHumidifier() {
        //Serial.println(F("HumidityRelayManager: Enabling the humidifier"));
        this->SetIndicatorColor(0, 255, 255);  // aqua
        digitalWrite(HumidityRelayManager::HumidifierControlPin, HIGH);
    }

    void HumidityRelayManager::DisableHumidifier() {
        //Serial.println(F("HumidityRelayManager: Disabling the humidifier"));
        digitalWrite(HumidityRelayManager::HumidifierControlPin, LOW);
    }

    void HumidityRelayManager::EnableDehumidifier() {
        //Serial.println(F("HumidityRelayManager: Enabling the dehumidifier"));
        this->SetIndicatorColor(255, 0, 0);  // red
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

        analogWrite(HumidityRelayManager::RedPin, red);
        analogWrite(HumidityRelayManager::GreenPin, green);
        analogWrite(HumidityRelayManager::BluePin, blue);  
    }
}
