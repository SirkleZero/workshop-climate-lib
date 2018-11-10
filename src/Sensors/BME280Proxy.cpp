#include <Adafruit_BME280.h>
#include "BME280Proxy.h"

namespace Sensors {
    namespace BME280 {
        BME280Proxy::BME280Proxy(BME280Proxy::TemperatureUnit units) {
            this->units = units;
        }

        void BME280Proxy::Initialize(){
            // initialize the BME280 sensor          
            if (!bme.begin(&Wire)) {
                Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
                while (1);
            }

            // configure the BME280 sensor, // suggested rate is 1/60Hz (1s)
            bme.setSampling(Adafruit_BME280::MODE_FORCED,
                        Adafruit_BME280::SAMPLING_X1,   // temperature
                        Adafruit_BME280::SAMPLING_X1, // pressure
                        Adafruit_BME280::SAMPLING_X1,   // humidity
                        Adafruit_BME280::FILTER_OFF);
        }

        bool BME280Proxy::ReadSensor(SensorData *data){
            bme.takeForcedMeasurement();

            switch(this->units){
                case C:
                    data->climate.Temperature = bme.readTemperature();
                    data->climate.Units = 'C';
                    break;
                case F:
                    data->climate.Temperature = ClimateData::ConvertCToF(bme.readTemperature());
                    data->climate.Units = 'F';
                    break;
                default:
                    data->climate.Temperature = bme.readTemperature();
                    data->climate.Units = 'C';
                    break;
            }

            data->climate.Pressure = bme.readPressure();
            data->climate.Humidity = bme.readHumidity();

            return true;
        }

        void BME280Proxy::PrintDebug() {
            SensorData data;
            this->ReadSensor(&data);

            Serial.print(F("Temperature = "));
            Serial.print(data.climate.Temperature);
            Serial.print(F(" *"));
            Serial.println(this->units);

            Serial.print(F("Pressure = "));
            Serial.print(data.climate.Pressure / 100.0F);
            Serial.println(F(" hPa")); // pressure in pascals

            Serial.print(F("Humidity = "));
            Serial.print(data.climate.Humidity + humidityOffset);
            Serial.println(F(" %"));

            Serial.println();
        }
    }
}
