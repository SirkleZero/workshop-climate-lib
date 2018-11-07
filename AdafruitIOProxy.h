/*
https://io.adafruit.com/api/docs/

*/
#include "SensorData.h"
#include "AdafruitIO_WiFi.h"
#include "Secrets.h"

using namespace Configuration;

namespace TX {
    #ifndef AdafruitIOProxy_h
        #define AdafruitIOProxy_h

        class AdafruitIOProxy {
            public:
                AdafruitIOProxy();
                ~AdafruitIOProxy();
                void Initialize(Secrets *secrets);
                void Transmit(SensorData data);
            private:
                Secrets *secrets;
                AdafruitIO_WiFi *io;

                AdafruitIO_Feed *temperatureFeed;
                AdafruitIO_Feed *humidityFeed;
                AdafruitIO_Feed *pressureFeed;

                AdafruitIO_Feed *pm10_standard;
                AdafruitIO_Feed *pm25_standard;
                AdafruitIO_Feed *pm100_standard;
                AdafruitIO_Feed *pm10_environment;
                AdafruitIO_Feed *pm25_environment;
                AdafruitIO_Feed *pm100_environment;
                AdafruitIO_Feed *particles_03um;
                AdafruitIO_Feed *particles_05um;
                AdafruitIO_Feed *particles_10um;
                AdafruitIO_Feed *particles_25um;
                AdafruitIO_Feed *particles_50um;
                AdafruitIO_Feed *particles_100um;

                void printWiFiStatus();
                void Disconnect();
        };

    #endif
}