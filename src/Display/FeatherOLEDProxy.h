#include "..\Sensors\SensorData.h"
#include "..\TX\TXResult.h"

namespace Display {
    #ifndef FeatherOLEDProxy_h
        #define FeatherOLEDProxy_h

        // library interface description
        class FeatherOLEDProxy {
            public:
                enum ButtonMode {
                    Default = 0x00,
                    A = 0x01,
                    B = 0x02,
                    C = 0x03
                };

                FeatherOLEDProxy();

                void Initialize();
                void PrintSensors(SensorData data);
                void PrintWaiting();
                void PrintRFM69Update(TXResult *result);
                void Clear();

                void ButtonA();
                void ButtonB();
                void ButtonC();

            // library-accessible "private" interface
            private:
                void PrintTemperature(SensorData *data, uint16_t color);
                void PrintHumidity(SensorData *data, uint16_t color);
                void PrintPressure(SensorData *data, uint16_t color);
                void PrintParticulates(SensorData *data, uint16_t color);
        };

    #endif
}
