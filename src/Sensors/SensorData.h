#ifndef SensorData_h
    #define SensorData_h

    #include "ClimateData.h"
    #include "ParticleData.h"

    struct SensorData {
        ClimateData climate;
        ParticleData particulates;

        void PrintDebug(){
            climate.PrintDebug();
            particulates.PrintDebug();
        }

        static SensorData EmptyData() {
            SensorData d;
            d.climate.Temperature = 0;
            d.climate.Humidity = 0;
            d.climate.Pressure = 0;

            d.particulates.pm10_standard = 0;
            d.particulates.pm25_standard = 0;
            d.particulates.pm100_standard = 0;
            d.particulates.pm10_env = 0;
            d.particulates.pm25_env = 0;
            d.particulates.pm100_env = 0;
            d.particulates.particles_03um = 0;
            d.particulates.particles_05um = 0;
            d.particulates.particles_10um = 0;
            d.particulates.particles_25um = 0;
            d.particulates.particles_50um = 0;
            d.particulates.particles_100um = 0;

            return d;
        }
    };

#endif
