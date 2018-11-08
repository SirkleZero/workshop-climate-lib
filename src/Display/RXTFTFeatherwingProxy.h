#include "..\Sensors\SensorData.h"
#include "..\Rectangle.h"

namespace Display {
    #ifndef RXTFTFeatherwingProxy_h
        #define RXTFTFeatherwingProxy_h

        class RXTFTFeatherwingProxy {
            public:
                RXTFTFeatherwingProxy();
                void Initialize();
                void PrintSensors(SensorData data);
                void DrawLayout();
                void Clear();
            private:
                uint16_t height;
                uint16_t width;
                SensorData previousData;

                Rectangle humidityArea;
                Rectangle temperatureArea;
                Rectangle particulateArea;
                Rectangle aqiLabelArea;
                Rectangle aqiIndicatorArea;
                AqiScaleRectangle aqiScaleGoodArea;
                AqiScaleRectangle aqiScaleModerateArea;
                AqiScaleRectangle aqiScaleUnhealthySensitiveArea;
                AqiScaleRectangle aqiScaleUnhealthyArea;
                AqiScaleRectangle aqiScaleVeryUnhealthyArea;
                AqiScaleRectangle aqiScaleHazardousArea;
                Rectangle aqiScaleStrokeArea;

                void PrintTemperature(SensorData *data, uint16_t color);
                void PrintHumidity(SensorData *data, uint16_t color);
                void PrintPressure(SensorData *data, uint16_t color);
                void PrintParticulates(SensorData *data, uint16_t color);

                int16_t GetCenteredPosition(char *text, int16_t x, int16_t y, int16_t areaWidth);
                void DrawAirQualityIndicator(SensorData *data, bool overwritting);
        };

    #endif
}
