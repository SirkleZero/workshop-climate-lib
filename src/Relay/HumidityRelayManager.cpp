#include "HumidityRelayManager.h"
#include "..\Sensors\SensorData.h"

using namespace Sensors;

namespace Relay {    
    HumidityRelayManager::HumidityRelayManager() { }

    void HumidityRelayManager::Initialize(){
        pinMode(this->ledPin, OUTPUT);
        pinMode(this->relayControlPin, OUTPUT);
        pinMode(this->switchPin, INPUT);
    }

    void HumidityRelayManager::SetRelayState(SensorData *data){
        Serial.println(digitalRead(switchPin));

        if(data->climate.Humidity > 48.0 || data->climate.Humidity < 43){
            // turn on the relay!
            this->relayState = HIGH;
        }else{
            // turn off the relay
            this->relayState = LOW;
        }

        digitalWrite(this->ledPin, this->relayState);
        digitalWrite(this->relayControlPin, this->relayState);
    }
}
