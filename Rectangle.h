#ifndef Rectangle_h
    #define Rectangle_h

    struct Rectangle {
        Rectangle(int16_t x, int16_t y, int16_t width, int16_t height) : x(x), y(y), width(width), height(height) { }
        int16_t x;
        int16_t y;
        int16_t height;
        int16_t width;

        void PrintDebug(){
            Serial.print("x: ");
            Serial.println(x);

            Serial.print("y: ");
            Serial.println(y);

            Serial.print("height: ");
            Serial.println(height);

            Serial.print("width: ");
            Serial.println(width);
        }
    };

    struct AqiScaleRectangle : Rectangle {
        AqiScaleRectangle(int16_t x, int16_t y, int16_t width, int16_t height, char *label, uint16_t color, int16_t minIndex, int16_t maxIndex) : Rectangle(x, y, width, height), Label(label), Color(color), MinIndex(minIndex), MaxIndex(maxIndex) { }

        char *Label;
        uint16_t Color;
        int16_t MinIndex;
        int16_t MaxIndex;

        bool IsAqiRectangleFor(int16_t aqiScore) {
            return aqiScore >= MinIndex && aqiScore <= MaxIndex;
        }

        int16_t GetVerticalIndicatorLocation(int16_t aqiScore) {
            float divisor = (MaxIndex - MinIndex + 1) / width;
            return ((aqiScore - MinIndex) / divisor) + x;
        }
    };
#endif