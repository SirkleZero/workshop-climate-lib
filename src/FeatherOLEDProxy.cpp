#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "FeatherOLEDProxy.h"
#include "SensorData.h"
#include "TXResult.h"

namespace Display {
    Adafruit_SSD1306 display = Adafruit_SSD1306();

    #if defined(ESP8266)
        #define BUTTON_A 0
        #define BUTTON_B 16
        #define BUTTON_C 2
        #define LED      0
    #elif defined(ESP32)
        #define BUTTON_A 15
        #define BUTTON_B 32
        #define BUTTON_C 14
        #define LED      13
    #elif defined(ARDUINO_STM32F2_FEATHER)
        #define BUTTON_A PA15
        #define BUTTON_B PC7
        #define BUTTON_C PC5
        #define LED PB5
    #elif defined(TEENSYDUINO)
        #define BUTTON_A 4
        #define BUTTON_B 3
        #define BUTTON_C 8
        #define LED 13
    #elif defined(ARDUINO_FEATHER52)
        #define BUTTON_A 31
        #define BUTTON_B 30
        #define BUTTON_C 27
        #define LED 17
    #else // 32u4, M0, M4, and 328p
        #define BUTTON_A 9
        #define BUTTON_B 6
        #define BUTTON_C 5
        #define LED      13
    #endif

    #if (SSD1306_LCDHEIGHT != 32)
        #error("Height incorrect, please fix Adafruit_SSD1306.h!");
    #endif

    FeatherOLEDProxy::ButtonMode displayMode = FeatherOLEDProxy::Default;
    SensorData previousData;
    FeatherOLEDProxy* interruptThis;

    FeatherOLEDProxy::FeatherOLEDProxy() { }

    void FeatherOLEDProxy::ButtonA(){
        if(displayMode != FeatherOLEDProxy::A) {
            displayMode = FeatherOLEDProxy::A;
            this->Clear();
            PrintSensors(previousData);
        }
    }

    void FeatherOLEDProxy::ButtonB() {
        if(displayMode != FeatherOLEDProxy::B) {
            displayMode = FeatherOLEDProxy::B;
            this->Clear();
            PrintSensors(previousData);
        }
    }

    void FeatherOLEDProxy::ButtonC() {
        if(displayMode != FeatherOLEDProxy::C) {
            displayMode = FeatherOLEDProxy::C;
            this->Clear();
            PrintSensors(previousData);
        }
    }

    void ButtonWrapperA() {
        interruptThis->ButtonA();
    }

    void ButtonWrapperB() {
        interruptThis->ButtonB();
    }

    void ButtonWrapperC() {
        interruptThis->ButtonC();
    }

    void FeatherOLEDProxy::Initialize(){
        Serial.println("OLED FeatherWing test");
        // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
        display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

        // init done
        Serial.println("OLED begun");

        // display the adafruit splash screen
        display.display();
        
        pinMode(BUTTON_A, INPUT_PULLUP);
        pinMode(BUTTON_B, INPUT_PULLUP);
        pinMode(BUTTON_C, INPUT_PULLUP);

        attachInterrupt(digitalPinToInterrupt(BUTTON_A), ButtonWrapperA, FALLING);
        attachInterrupt(digitalPinToInterrupt(BUTTON_B), ButtonWrapperB, FALLING);
        attachInterrupt(digitalPinToInterrupt(BUTTON_C), ButtonWrapperC, FALLING);
    }

    void FeatherOLEDProxy::Clear(){
        display.clearDisplay();
        display.display();
    }

    void FeatherOLEDProxy::PrintWaiting() {
        this->Clear();

        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,0);

        display.println("waiting on sensors...");
        display.display();
    }

    void FeatherOLEDProxy::PrintSensors(SensorData data) {
        display.setTextSize(1);

        switch(displayMode){
            case A:
                // print out the temperature
                this->PrintTemperature(&previousData, BLACK);
                this->PrintTemperature(&data, WHITE);

                //print out the humidity
                this->PrintHumidity(&previousData, BLACK);
                this->PrintHumidity(&data, WHITE);

                //print out the pressure
                this->PrintPressure(&previousData, BLACK);
                this->PrintPressure(&data, WHITE);
                break;
            case B:
                // print out the particulate information
                this->PrintParticulates(&previousData, BLACK);
                this->PrintParticulates(&data, WHITE);
                break;
            case C:
                this->Clear();
                break;
            case Default:
                // psuedo recursion
                displayMode = FeatherOLEDProxy::A;
                PrintSensors(data);
                break;
        }

        display.display();
        previousData = data;
    }

    void FeatherOLEDProxy::PrintTemperature(SensorData *data, uint16_t color) {
        display.setCursor(0,0);
        display.setTextColor(color);
        display.print("Temperature: ");
        display.print(data->climate.Temperature);
        display.print(" ");
        display.print((char)247); // this is the ascii degrees character as displayed by the feather oled display
        display.print("F");
    }

    void FeatherOLEDProxy::PrintHumidity(SensorData *data, uint16_t color) {
        display.setCursor(0,8);
        display.setTextColor(color);
        display.print("Humidity: ");
        display.print(data->climate.Humidity);
        display.print("% RH");
    }

    void FeatherOLEDProxy::PrintPressure(SensorData *data, uint16_t color) {
        display.setCursor(0,16);
        display.setTextColor(color);
        display.print("Pressure: ");
        display.print(data->climate.Pressure / 100.0F);
        display.print(" hPa");
    }

    void FeatherOLEDProxy::PrintParticulates(SensorData *data, uint16_t color){
        display.setTextColor(color);

        display.setCursor(0, 0);
        display.println(":Values in microns:");

        display.setCursor(0, 8);
        display.print(".3u: ");
        display.print(data->particulates.particles_03um);
        display.print(" ");
        display.print(".5u: ");
        display.print(data->particulates.particles_05um);

        display.setCursor(0, 16);
        display.print("1u: ");
        display.print(data->particulates.particles_10um);
        display.print(" ");
        display.print("2.5u: ");
        display.print(data->particulates.particles_25um);

        display.setCursor(0, 24);
        display.print("5u: ");
        display.print(data->particulates.particles_50um);
        display.print(" ");
        display.print("10u: ");
        display.print(data->particulates.particles_100um);

        //display.startscrolldiagright(0x0F, 0x0F);
    }

    void FeatherOLEDProxy::PrintRFM69Update(TXResult *result){
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 24);

        if(result->TransmitSuccessful){
            display.print("RSSI: ");
            display.println(result->RSSI);
        }else{
            display.println("Transmission failed!");
        }

        display.display();
    }
}
