#include "HumidityRelayManager.h"

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

    }

    void HumidityRelayManager::SetRelayState(SensorData *data) {
        // re-write this to handle two different relays.
        // one for humidification
        // one for dehumidification

        if(data->climate.Humidity > this->configuration->MaximumHumidity || data->climate.Humidity < this->configuration->MinimumHumidity) {
            this->relayState = HIGH;
            // digitalWrite(this->ledPin, this->relayState);
            // digitalWrite(this->relayControlPin, this->relayState);
            Serial.println("relay on");
        } else {
            this->relayState = LOW;
            // digitalWrite(this->ledPin, this->relayState);
            // digitalWrite(this->relayControlPin, this->relayState);
            Serial.println("relay off");
        }

        
    }
}
