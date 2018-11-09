#ifndef ParticleData_h
    #define ParticleData_h

    #include <Arduino.h>

    struct ParticleData {
        uint16_t pm10_standard;
        uint16_t pm25_standard;
        uint16_t pm100_standard;
        uint16_t pm10_env;
        uint16_t pm25_env;
        uint16_t pm100_env;
        uint16_t particles_03um;
        uint16_t particles_05um;
        uint16_t particles_10um;
        uint16_t particles_25um;
        uint16_t particles_50um;
        uint16_t particles_100um;

        void PrintDebug(){
            Serial.println("---------------------------------------");
            Serial.println("Concentration Units (standard)");
            Serial.print("PM 1.0: "); Serial.print(pm10_standard);
            Serial.print("\t\tPM 2.5: "); Serial.print(pm25_standard);
            Serial.print("\t\tPM 10: "); Serial.println(pm100_standard);
            Serial.println("---------------------------------------");
            Serial.println("Concentration Units (environmental)");
            Serial.print("PM 1.0: "); Serial.print(pm10_env);
            Serial.print("\t\tPM 2.5: "); Serial.print(pm25_env);
            Serial.print("\t\tPM 10: "); Serial.println(pm100_env);
            Serial.println("---------------------------------------");
            Serial.print("Particles > 0.3um / 0.1L air:"); Serial.println(particles_03um);
            Serial.print("Particles > 0.5um / 0.1L air:"); Serial.println(particles_05um);
            Serial.print("Particles > 1.0um / 0.1L air:"); Serial.println(particles_10um);
            Serial.print("Particles > 2.5um / 0.1L air:"); Serial.println(particles_25um);
            Serial.print("Particles > 5.0um / 0.1L air:"); Serial.println(particles_50um);
            Serial.print("Particles > 10.0 um / 0.1L air:"); Serial.println(particles_100um);
            Serial.println("---------------------------------------");
            Serial.println();
        }
    };

#endif
