#include "HumidityRelayManager.h"

using namespace Sensors;

namespace Relay {    
    HumidityRelayManager::HumidityRelayManager() { }

    void HumidityRelayManager::Initialize(ControllerConfiguration *configuration) {
        this->configuration = configuration;

        pinMode(this->humidificationModeLedPin, OUTPUT);
        pinMode(this->dehumidificationModeLedPin, OUTPUT);
        pinMode(this->humidifierControlPin, OUTPUT);
        pinMode(this->dehumidifierControlPin, OUTPUT);
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
            this->ShutDown();
        }
    }

    void HumidityRelayManager::EnableHumidifier() {
        Serial.println(F("HumidityRelayManager: Enabling the humidifier"));
        digitalWrite(this->humidificationModeLedPin, HIGH);
        digitalWrite(this->humidifierControlPin, HIGH);
    }

    void HumidityRelayManager::DisableHumidifier() {
        Serial.println(F("HumidityRelayManager: Disabling the humidifier"));
        digitalWrite(this->humidificationModeLedPin, LOW);
        digitalWrite(this->humidifierControlPin, LOW);
    }

    void HumidityRelayManager::EnableDehumidifier() {
        Serial.println(F("HumidityRelayManager: Enabling the dehumidifier"));
        digitalWrite(this->dehumidificationModeLedPin, HIGH);
        digitalWrite(this->dehumidifierControlPin, HIGH);
    }

    void HumidityRelayManager::DisableDehumidifier() {
        Serial.println(F("HumidityRelayManager: Disabling the dehumidifier"));
        digitalWrite(this->dehumidificationModeLedPin, LOW);
        digitalWrite(this->dehumidifierControlPin, LOW);
    }

    void HumidityRelayManager::ShutDown() {
        Serial.println(F("HumidityRelayManager: Goldilocks! Shutting down humidifier AND dehumidifier!"));
        this->DisableHumidifier();
        this->DisableDehumidifier();
    }
}
