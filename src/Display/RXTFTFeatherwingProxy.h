#ifndef RXTFTFeatherwingProxy_h
    #define RXTFTFeatherwingProxy_h

    // external dependencies
    #include <Adafruit_GFX.h>
    #include <Adafruit_ILI9341.h>

    // include some fonts
    #include <Fonts\FreeSansBold24pt7b.h>
    #include <Fonts\FreeSansBold12pt7b.h>
    #include <Fonts\FreeSansBold9pt7b.h>
    #include <Fonts\FreeSans9pt7b.h>

    #include "..\Sensors\SensorData.h"
    #include "..\Rectangle.h"

    using namespace Sensors;

    namespace Display {
        class RXTFTFeatherwingProxy {
            public:
                RXTFTFeatherwingProxy();
                void Initialize();
                void PrintSensors(SensorData data);
                void PrintFreeMemory(int freeMemory);
                void DrawLayout();
                void Clear();
            private:
                // labels
                const char *aqiScaleGoodLabel = "Good";
                const char *aqiScaleModerateLabel = "Moderate";
                const char *aqiScaleUnhealthySensitiveLabel = "Sensitive";
                const char *aqiScaleUnhealthyLabel = "Unhealthy";
                const char *aqiScaleVeryUnhealthyLabel = "Very Unhealthy";
                const char *aqiScaleHazardousLabel = "Hazardous";

                // colors
                const uint16_t readingsTextColor = 0x5DDF;
                const uint16_t layouttextColor = ILI9341_WHITE;
                const uint16_t layoutlineColor = ILI9341_WHITE;
                const uint16_t backgroundColor = ILI9341_BLACK;
                const uint16_t aqiGood = ILI9341_GREEN;
                const uint16_t aqiModerate = ILI9341_YELLOW;
                const uint16_t aqiUnhealthySensitive = ILI9341_ORANGE;
                const uint16_t aqiUnhealthy = ILI9341_RED;
                const uint16_t aqiVeryUnhealthy = 0xB800;
                const uint16_t aqiHazardous = 0x8000;

                const uint8_t ROTATION_HORIZONTAL = 1; 

                Adafruit_ILI9341 tft;
                uint16_t height;
                uint16_t width;
                SensorData previousData;
                int previousFreeMemory = 0;

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
                int16_t GetCenteredPosition(const char *text, int16_t x, int16_t y, int16_t areaWidth);
                void DrawAirQualityIndicator(SensorData *data, bool overwritting);
        };
    }

#endif
