#include "AdafruitIOProxy.h"

using namespace Sensors;
using namespace Configuration;

namespace TX {
    AdafruitIOProxy::AdafruitIOProxy()  { }

    AdafruitIOProxy::~AdafruitIOProxy() {
        delete io;
    }

    void AdafruitIOProxy::Initialize(Secrets *secrets) {
        this->secrets = secrets;

        io = new AdafruitIO_WINC1500(this->secrets->adafruitio_username, this->secrets->adafruitio_key, this->secrets->wifi_ssid, this->secrets->wifi_password);
        
        temperatureFeed = io->feed("workshop-climate.temperature");
        humidityFeed = io->feed("workshop-climate.humidity");
        pressureFeed = io->feed("workshop-climate.pressure");

        pm10_standard = io->feed("workshop-climate.pm10-standard");
        pm25_standard = io->feed("workshop-climate.pm25-standard");
        pm100_standard = io->feed("workshop-climate.pm100-standard");
        pm10_environment = io->feed("workshop-climate.pm10-environment");
        pm25_environment = io->feed("workshop-climate.pm25-environment");
        pm100_environment = io->feed("workshop-climate.pm100-environment");
        particles_03um = io->feed("workshop-climate.particles-03um");
        particles_05um = io->feed("workshop-climate.particles-05um");
        particles_10um = io->feed("workshop-climate.particles-10um");
        particles_25um = io->feed("workshop-climate.particles-25um");
        particles_50um = io->feed("workshop-climate.particles-50um");
        particles_100um = io->feed("workshop-climate.particles-100um");
    }

    void AdafruitIOProxy::Disconnect() {
        WiFi.disconnect();
        WiFi.end();
    }

    IoTUploadResult AdafruitIOProxy::Transmit(SensorData data) {
        IoTUploadResult result;

        Serial.println(F("connecting to wifi via Adafruit IO library"));
        io->connect(); // this is just connecting to the wifi. it's just a wrapper to the underlying wifi system.

        // wait for a connection, but not forever yo! 15 seconds should be good enough?
        // calling io->status() actually does a lot behind the scenes, and can return
        // quite a few different status's. We only care about being connected though.
        //byte secondsToWait = 15;
        //unsigned long connectionDelay = 500;
        float iterations = AdafruitIOProxy::SecondsToWait * (1000 / AdafruitIOProxy::ConnectionDelay);
        while (io->status() < AIO_CONNECTED && iterations > 0) {
            Serial.print(F("."));
            delay(AdafruitIOProxy::ConnectionDelay);
            iterations--;
        }

        if(iterations <= 0) {
            // we exceeded our timeout period. return a failure.
            result.ErrorMessage = io->statusText();
            this->Disconnect();
            return result;
        }

        // send climate information to Adafruit IO
        temperatureFeed->save(data.climate.Temperature);
        humidityFeed->save(data.climate.Humidity);
        pressureFeed->save(data.climate.Pressure / 100); // send it in hPa

        // send particulates information to Adafruit IO
        pm10_standard->save(data.particulates.pm10_standard);
        pm25_standard->save(data.particulates.pm25_standard);
        pm100_standard->save(data.particulates.pm100_standard);
        pm10_environment->save(data.particulates.pm10_env);
        pm25_environment->save(data.particulates.pm25_env);
        pm100_environment->save(data.particulates.pm100_env);
        particles_03um->save(data.particulates.particles_03um);
        particles_05um->save(data.particulates.particles_05um);
        particles_10um->save(data.particulates.particles_10um);
        particles_25um->save(data.particulates.particles_25um);
        particles_50um->save(data.particulates.particles_50um);
        particles_100um->save(data.particulates.particles_100um);

        Serial.println(F("sending data to Adafruit IO..."));

        // send the queued up data points. run commits the transaction (essentially).
        io->run();
        result.IsSuccess = true;

        Serial.println(F("data sent to adafruit!"));

        // get information about the wifi connection for use on the display
        result.SSID = WiFi.SSID();
        result.RSSI = WiFi.RSSI();
        result.LocalIP = WiFi.localIP();
        result.GatewayIP = WiFi.gatewayIP();
        result.SubnetMask = WiFi.subnetMask();
        result.ErrorMessage = F("");

        this->Disconnect();
        
        return result;
    }
}
