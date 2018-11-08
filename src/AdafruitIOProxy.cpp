#include "AdafruitIO_WiFi.h"
#include "AdafruitIOProxy.h"
#include <Wire.h>
#include "Configuration/Secrets.h"

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

        Serial.println("turned off WiFi");
    }

    void AdafruitIOProxy::Transmit(SensorData data) {
        Serial.println("sending data to adafruit");
        io->connect();

        // wait for a connection
        while (io->status() < AIO_CONNECTED) {
            Serial.print(".");
            delay(500);
        }

        Serial.println(io->statusText());

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

        io->run(); // send the queued up data points

        this->Disconnect();

        Serial.println("data sent to adafruit!");
    }

    void AdafruitIOProxy::printWiFiStatus() {
        // print the SSID of the network you're attached to:
        Serial.print("SSID: ");
        Serial.println(WiFi.SSID());

        // print your WiFi shield's IP address:
        IPAddress ip = WiFi.localIP();
        Serial.print("IP Address: ");
        Serial.println(ip);

        // print the received signal strength:
        long rssi = WiFi.RSSI();
        Serial.print("signal strength (RSSI):");
        Serial.print(rssi);
        Serial.println(" dBm");
    }
}
