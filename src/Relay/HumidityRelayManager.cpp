#include "HumidityRelayManager.h"

using namespace Sensors;

namespace Relay {    
    HumidityRelayManager::HumidityRelayManager() :
        Orange(0xe6, 0x5c, 0x00),
        Green(0x33, 0xcc, 0x33),
        Purple(0x66, 0x00, 0xcc),
        Aqua(0x00, 0x99, 0xff),
        Red(0x99, 0x00, 0x00)
    { }

    void HumidityRelayManager::Initialize(ControllerConfiguration *configuration) {
        this->configuration = configuration;

        // relay control pins
        pinMode(HumidityRelayManager::HumidifierControlPin, OUTPUT);
        pinMode(HumidityRelayManager::DehumidifierControlPin, OUTPUT);
        
        this->EnableIndicator();
        
        this->SetIndicatorColor(HumidityRelayManager::Orange);
    }

    void HumidityRelayManager::KeepAlive() {
        this->previousKeepAliveCall = millis();
    }

    void HumidityRelayManager::AdjustClimate(SensorData data) {
        this->KeepAlive();

        // figure out which mode we need to be in. Humidification or Dehumidification.
        if(data.climate.Humidity > this->configuration->MaximumHumidity){
            // it's too humid, enable the dehumidifier, but only run it until we
			// hit the target humidity, otherwise we could overshoot.

			// TODO: adjust this to accomodate the above comment

            this->DisableHumidifier();
            this->EnableDehumidifier();
        } else if (data.climate.Humidity < this->configuration->MinimumHumidity){
            // it's too dry, enable the humidifier, but only run it until we
			// hit the target humidity, otherwise we could overshoot.
            this->DisableDehumidifier();
            this->EnableHumidifier();
        } else {
            // goldilocks zone, shut them both down
            this->ShutDown();
            this->SetIndicatorColor(HumidityRelayManager::Green);
        }
    }

    void HumidityRelayManager::EmergencyShutoff() {
        currentMillis = millis();
        if(this->currentMillis - this->previousKeepAliveCall >= this->configuration->RunawayTimeLimit) {
            // we exceeded our time! Shut it down!
            this->ShutDown();
            this->SetIndicatorColor(HumidityRelayManager::Purple);
        }
    }

    void HumidityRelayManager::EnableHumidifier() {
        this->SetIndicatorColor(HumidityRelayManager::Aqua);
        digitalWrite(HumidityRelayManager::HumidifierControlPin, HIGH);
    }

    void HumidityRelayManager::DisableHumidifier() {
        digitalWrite(HumidityRelayManager::HumidifierControlPin, LOW);
    }

    void HumidityRelayManager::EnableDehumidifier() {
        this->SetIndicatorColor(HumidityRelayManager::Red);
        digitalWrite(HumidityRelayManager::DehumidifierControlPin, HIGH);
    }

    void HumidityRelayManager::DisableDehumidifier() {
        digitalWrite(HumidityRelayManager::DehumidifierControlPin, LOW);
    }

    void HumidityRelayManager::ShutDown() {
        this->DisableHumidifier();
        this->DisableDehumidifier();
    }

    void HumidityRelayManager::SetIndicatorColor(RGB color) {
        this->EnableIndicator();

        if(HumidityRelayManager::CommonAnode) {
            analogWrite(HumidityRelayManager::RedPin, 255 - color.Red);
            analogWrite(HumidityRelayManager::GreenPin, 255 - color.Green);
            analogWrite(HumidityRelayManager::BluePin, 255 - color.Blue);
        } else {
            analogWrite(HumidityRelayManager::RedPin, color.Red);
            analogWrite(HumidityRelayManager::GreenPin, color.Green);
            analogWrite(HumidityRelayManager::BluePin, color.Blue);
        }
    }

    void HumidityRelayManager::DisableIndicator() {
        if(this->relayEnabled) {
            pinMode(HumidityRelayManager::RedPin, INPUT);
            pinMode(HumidityRelayManager::GreenPin, INPUT);
            pinMode(HumidityRelayManager::BluePin, INPUT);

            this->relayEnabled = false;
        }
    }

    void HumidityRelayManager::EnableIndicator() {
        if(!this->relayEnabled) {
            pinMode(HumidityRelayManager::RedPin, OUTPUT);
            pinMode(HumidityRelayManager::GreenPin, OUTPUT);
            pinMode(HumidityRelayManager::BluePin, OUTPUT);

            this->relayEnabled = true;
        }
    }
}
