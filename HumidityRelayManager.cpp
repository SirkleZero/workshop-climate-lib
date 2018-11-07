#include <Arduino.h>
#include "HumidityRelayManager.h"

namespace Relay {
    const int ledPin =  LED_BUILTIN;// the number of the LED pin
    const int relayControlPin = 5;
    const int switchPin = 9;

    int relayState = LOW;

    // Constructor
    HumidityRelayManager::HumidityRelayManager() { }

    void HumidityRelayManager::Initialize(){
        pinMode(ledPin, OUTPUT);
        pinMode(relayControlPin, OUTPUT);
        pinMode(switchPin, INPUT);
    }

    void HumidityRelayManager::SetRelayState(SensorData *data){
        Serial.println(digitalRead(switchPin));

        Serial.print(data->climate.Humidity);
        Serial.print(": ");

        if(data->climate.Humidity > 48.0 || data->climate.Humidity < 43){
            // turn on the relay!
            relayState = HIGH;
            Serial.println("relay on");
        }else{
            // turn off the relay
            relayState = LOW;
            Serial.println("relay off");
        }

        digitalWrite(ledPin, relayState);
        digitalWrite(relayControlPin, relayState);
    }
}
