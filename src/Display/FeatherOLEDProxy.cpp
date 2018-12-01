#include "FeatherOLEDProxy.h"

using namespace Sensors;
using namespace TX;

namespace Display {
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

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
#error(F("Height incorrect, please fix Adafruit_SSD1306.h!"));
#endif

	// these three variables exist in global scope because they are used to handle the interrupts for the buttons used by the application.
	
	// displayMode, previousData, previousResult, and previousError exist here because of the need to have the button interrupts happen on a pointer to the OLED display proxy rather than on an instance of it. If they were an instance of it, then we'd have problems accessing their values.
	FeatherOLEDProxy* interruptThis;
	ButtonMode displayMode = ButtonMode::Default;
	SensorData previousData;
	TXResult previousResult;
	char *previousError;	

	// declare the display object. For whatever reason, this doesn't work as a class variable.
	Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

	/// <summary>Initializes a new instance of the <see cref="FeatherOLEDProxy"/> class.</summary>
	FeatherOLEDProxy::FeatherOLEDProxy() {}

	/// <summary>Handles the press of the 'A' button, which places the display into a specific display mode.</summary>
	void FeatherOLEDProxy::ButtonA()
	{
		if (displayMode != ButtonMode::A)
		{
			displayMode = ButtonMode::A;
			this->Clear();
			PrintSensors(previousData);
		}
	}

	/// <summary>Handles the press of the 'B' button, which places the display into a specific display mode.</summary>
	void FeatherOLEDProxy::ButtonB()
	{
		if (displayMode != ButtonMode::B)
		{
			displayMode = ButtonMode::B;
			this->Clear();
			PrintSensors(previousData);
		}
	}

	/// <summary>Handles the press of the 'C' button, which places the display into a specific display mode.</summary>
	void FeatherOLEDProxy::ButtonC()
	{
		if (displayMode != ButtonMode::C)
		{
			displayMode = ButtonMode::C;
			this->Clear();
			PrintSensors(previousData);
		}
	}

	/// <summary>Handles the interrupt for the click of the 'A' button.</summary>
	void ButtonWrapperA()
	{
		interruptThis->ButtonA();
	}

	/// <summary>Handles the interrupt for the click of the 'B' button.</summary>
	void ButtonWrapperB()
	{
		interruptThis->ButtonB();
	}

	/// <summary>Handles the interrupt for the click of the 'C' button.</summary>
	void ButtonWrapperC()
	{
		interruptThis->ButtonC();
	}

	/// <summary>Executes initialization logic for the object.</summary>
	/// <returns>An <see cref="InitializationResult"/> that describes the result of initialization.</returns>
	InitializationResult FeatherOLEDProxy::Initialize()
	{
		InitializationResult result;

		// initialize with the I2C addr 0x3C (for the 128x32)
		if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
		{
			result.ErrorMessage = F("Failed to initialize the Feather OLED display.");
			return result;
		}

		// display the adafruit splash screen
		display.display();

		// set up the displays built in buttons using the built in pull up resistors.
		pinMode(BUTTON_A, INPUT_PULLUP);
		pinMode(BUTTON_B, INPUT_PULLUP);
		pinMode(BUTTON_C, INPUT_PULLUP);

		// set up the interrupts for the buttons so that when they are clicked we can capture the event and process the click.
		attachInterrupt(digitalPinToInterrupt(BUTTON_A), ButtonWrapperA, FALLING);
		attachInterrupt(digitalPinToInterrupt(BUTTON_B), ButtonWrapperB, FALLING);
		attachInterrupt(digitalPinToInterrupt(BUTTON_C), ButtonWrapperC, FALLING);

		result.IsSuccessful = true;
		return result;
	}

	/// <summary>Clears all displayed content from the screen.</summary>
	void FeatherOLEDProxy::Clear()
	{
		display.clearDisplay();
		display.display();
	}

	/// <summary>Prints a waiting message to the display.</summary>
	void FeatherOLEDProxy::PrintWaiting()
	{
		this->Clear();

		display.setTextSize(1);
		display.setTextColor(WHITE);
		display.setCursor(0, 0);

		display.println(F("waiting on sensors..."));
		display.display();
	}

	void FeatherOLEDProxy::PrintError(const __FlashStringHelper *message)
	{
		this->Clear();

		display.setTextSize(1);
		display.setTextColor(WHITE);
		display.setCursor(0, 0);

		display.println(message);
		display.display();
	}

	/// <summary>Prints sensor information to the screen.</summary>
	/// <param name="data">The <see cref="SensorData"> containing readings from the sensors.</param>
	void FeatherOLEDProxy::PrintSensors(SensorData data)
	{
		display.setTextSize(1);

		switch (displayMode)
		{
			case A:
				//print out the humidity
				this->PrintHumidity(&previousData, BLACK);
				this->PrintHumidity(&data, WHITE);

				// print out the temperature
				this->PrintTemperature(&previousData, BLACK);
				this->PrintTemperature(&data, WHITE);

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
				displayMode = ButtonMode::A;
				PrintSensors(data);
				break;
		}

		display.display();
		previousData = data;
	}

	/// <summary>Prints RFM69 status information to the display.</summary>
	/// <param name="result">The <see cref="TXResult"> containing the RFM69 result.</param>
	void FeatherOLEDProxy::PrintTransmissionInfo(TXResult result)
	{
		display.setTextSize(1);
		this->PrintRSSI(&previousResult, BLACK);

		if (result.TransmitSuccessful)
		{
			this->PrintRSSI(&result, WHITE);
		}
		else
		{
			display.println(F("Transmission failed!"));
		}

		display.display();
		previousResult = result;
	}

	void FeatherOLEDProxy::PrintRSSI(TXResult *result, uint16_t color)
	{
		display.setCursor(0, 24);
		display.setTextColor(color);
		display.print(F("RSSI: "));
		display.print(result->RSSI);
	}

	void FeatherOLEDProxy::PrintHumidity(SensorData *data, uint16_t color)
	{
		display.setCursor(0, 0);
		display.setTextColor(color);
		display.print(F("Humidity: "));
		display.print(data->Climate.Humidity);
		display.print(F("% RH"));
	}

	void FeatherOLEDProxy::PrintTemperature(SensorData *data, uint16_t color)
	{
		display.setCursor(0, 8);
		display.setTextColor(color);
		display.print(F("Temperature: "));
		display.print(data->Climate.Temperature);
		display.print(F(" "));
		display.print((char)247); // this is the ascii degrees character as displayed by the feather oled display
		display.print(F("F"));
	}

	void FeatherOLEDProxy::PrintPressure(SensorData *data, uint16_t color)
	{
		display.setCursor(0, 16);
		display.setTextColor(color);
		display.print(F("Pressure: "));
		display.print(data->Climate.Pressure / 100.0F);
		display.print(F(" hPa"));
	}

	void FeatherOLEDProxy::PrintParticulates(SensorData *data, uint16_t color)
	{
		display.setTextColor(color);

		display.setCursor(0, 0);
		display.println(F(":Values in microns:"));

		display.setCursor(0, 8);
		display.print(F(".3u: "));
		display.print(data->Particulates.particles_03um);
		display.print(F(" "));
		display.print(F(".5u: "));
		display.print(data->Particulates.particles_05um);

		display.setCursor(0, 16);
		display.print(F("1u: "));
		display.print(data->Particulates.particles_10um);
		display.print(F(" "));
		display.print(F("2.5u: "));
		display.print(data->Particulates.particles_25um);

		display.setCursor(0, 24);
		display.print(F("5u: "));
		display.print(data->Particulates.particles_50um);
		display.print(F(" "));
		display.print(F("10u: "));
		display.print(data->Particulates.particles_100um);

		//display.startscrolldiagright(0x0F, 0x0F);
	}
}
