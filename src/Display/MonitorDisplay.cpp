#include "MonitorDisplay.h"

using namespace Sensors;

namespace Display {
#pragma region constructor and initialization

	/// <summary>Initializes a new instance of the <see cref="MonitorDisplay"/> class.</summary>
	MonitorDisplay::MonitorDisplay() :
		tft(TFT_CS, TFT_DC),
		ts(STMPE_CS),
		humidityArea(240, 85, 240, 170, ScreenRegion::Home),
		temperatureArea(0, 85, 240, 170, ScreenRegion::Home),
		settingsButton(435, 0, 45, 45, ScreenRegion::Home),
		homeButton(0, 0, 45, 45, ScreenRegion(ScreenRegion::Humidity | ScreenRegion::Settings | ScreenRegion::Temperature)),
		centeredMessageBox(0, 160, 480, 10)
	{}

	/// <summary>Executes initialization logic for the object.</summary>
	/// <returns>An <see cref="InitializationResult"/> that describes the result of initialization.</returns>
	InitializationResult MonitorDisplay::Initialize()
	{
		InitializationResult result;

		tft.begin();

		noInterrupts();
		tft.setRotation(MonitorDisplay::ScreenOrientation);
		interrupts();

		this->height = tft.height();
		this->width = tft.width();

		this->touchscreenExists = ts.begin();

		result.IsSuccessful = true;
		return result;
	}

#pragma endregion

	/// <summary>Clears all displayed content from the screen by wiping it with a black background.</summary>
	void MonitorDisplay::Clear()
	{
		noInterrupts();
		tft.fillScreen(MonitorDisplay::BackgroundColor);
		interrupts();
	}

	void MonitorDisplay::Display()
	{
		// use the Touched() function to determine if a screen area has been pressed, and if so, where.
		this->Display(this->Touched());
	}

	void MonitorDisplay::Display(ScreenRegion region)
	{
		// Depending on which screen region needs to be displayed, update the display.
		// the call to touched will return ScreenRegion "none" as its default, since if
		// nothing was clicked on, well, that would make sense right? But, we don't want 
		// this Display() method to perpetually update the display, so we ignore values
		// of none, instead only focusing on if something was actually clicked in a
		// valid area.
		// We also check that actual data was updated which requires a redraw of the screen.
		this->selectedRegion = region;
		this->regionChanged = this->selectedRegion != ScreenRegion::None && this->selectedRegion != this->activeRegion;
		if (this->DisplayUpdatable())
		{
			// nothing was actually clicked on, stay on the view we are currently rendering
			if (region != ScreenRegion::None)
			{
				this->activeRegion = this->selectedRegion;
			}

			switch (this->activeRegion)
			{
				case ScreenRegion::Home:
					Serial.println(F("ScreenRegion::Home"));
					this->DisplayHomeScreen();
					break;
				case ScreenRegion::Humidity:
					Serial.println(F("ScreenRegion::Humidity"));
					this->DisplayHumidityScreen();
					break;
				case ScreenRegion::Settings:
					Serial.println(F("ScreenRegion::Settings"));
					this->DisplaySettingsScreen();
					break;
				case ScreenRegion::StatusMessage:
					Serial.println(F("ScreenRegion::StatusMessage"));
					this->DisplayMessageScreen();
					break;
				case ScreenRegion::Temperature:
					Serial.println(F("ScreenRegion::Temperature"));
					this->DisplayTemperatureScreen();
					break;
				default:
					Serial.println(F("default"));
					this->DisplayHomeScreen();
					break;
			}

			// configure the button areas per the region we've clicked on / screen we are displaying.
			this->ConfigureButtons(this->activeRegion);

			// once the display has been updated, reset the state variables that track change status
			this->DisplayUpdated();
		}
	}

	void MonitorDisplay::ConfigureButtons(ScreenRegion region)
	{
		this->humidityArea.Enable(region);
		this->temperatureArea.Enable(region);
		this->settingsButton.Enable(region);
		this->homeButton.Enable(region);
	}

	bool MonitorDisplay::DisplayUpdatable()
	{
		return this->dataChanged || this->regionChanged;
	}

	void MonitorDisplay::DisplayUpdated()
	{
		this->dataChanged = false;
		this->regionChanged = false;
	}

	/// <summary></summary>
	ScreenRegion MonitorDisplay::Touched()
	{
		if (!this->ts.bufferEmpty())
		{
			// Convert the point that was clicked to compensate for the screen orientation. While
			// the display knows it's orientation, the touchscreen does not.
			TS_Point p = this->ts.getPoint();

			// NOTE: this 3.5" calculation is different than the one for the 2.4" display
			// not entirely sure why the larger display version behaves differently than the
			// smaller version.
			int y = map(p.x, MonitorDisplay::TSMaxY, MonitorDisplay::TSMinY, 0, tft.height());
			int x = map(p.y, MonitorDisplay::TSMinX, MonitorDisplay::TSMaxX, 0, tft.width());

			if (this->humidityArea.Contains(x, y))
			{
				return ScreenRegion::Humidity;
			}
			if (this->temperatureArea.Contains(x, y))
			{
				return ScreenRegion::Temperature;
			}
			if (this->settingsButton.Contains(x, y))
			{
				return ScreenRegion::Settings;
			}
			if (this->homeButton.Contains(x, y))
			{
				return ScreenRegion::Home;
			}
		}

		return ScreenRegion::None;
	}

	void MonitorDisplay::LoadMessage(const __FlashStringHelper* message)
	{
		this->currentMessage = message;
		this->dataChanged = true;
	}

	/// <summary>Prints sensor information to the screen.</summary>
	/// <param name="data">The <see cref="BME280Data"> containing readings from the sensors.</param>
	void MonitorDisplay::LoadSensorData(BME280Data data)
	{
		this->currentBME280Data = data;
		this->dataChanged = true;
	}

	/// <summary>Draws the initial static elements of the display.</summary>
	void MonitorDisplay::LayoutHomeScreen()
	{
		if (this->regionChanged)
		{
			Serial.println(F("laying out home screen"));
			noInterrupts();

			tft.fillScreen(MonitorDisplay::BackgroundColor);

			// for centering
			int16_t centeredTextXPosition;

			// draw our centerline
			tft.drawFastVLine(240, 30, 260, MonitorDisplay::LayoutLineColor);

			// set font
			tft.setFont(&calibrib12pt7b);
			tft.setTextColor(MonitorDisplay::LayoutTextColor);
			tft.setTextSize(1);

			// for temperature
			char* temperatureLabel = "Fahrenheit";
			//tft.drawRect(temperatureArea.x, temperatureArea.y, temperatureArea.width, temperatureArea.height, MonitorDisplay::LayoutLineColor);
			centeredTextXPosition = GetCenteredPosition(temperatureLabel, temperatureArea.x, temperatureArea.y, temperatureArea.width);
			tft.setCursor(centeredTextXPosition, temperatureArea.y + temperatureArea.height - 5);
			tft.println(temperatureLabel);

			// for Humidity
			char* humidityLabel = "% Humidity";
			//tft.drawRect(humidityArea.x, humidityArea.y, humidityArea.width, humidityArea.height, MonitorDisplay::LayoutLineColor);
			centeredTextXPosition = GetCenteredPosition(humidityLabel, humidityArea.x, humidityArea.y, humidityArea.width);
			tft.setCursor(centeredTextXPosition, humidityArea.y + humidityArea.height - 5);
			tft.println(humidityLabel);

			// draw the box for the settings screen
			//tft.drawRect(settingsButton.x, settingsButton.y, settingsButton.width, settingsButton.height, MonitorDisplay::LayoutLineColor);

			// draw the box for the home screen
			//tft.drawRect(homeButton.x, homeButton.y, homeButton.width, homeButton.height, MonitorDisplay::LayoutLineColor);

			interrupts();
		}
	}

	void MonitorDisplay::LayoutHumidityScreen()
	{
		if (this->regionChanged)
		{
			Serial.println(F("laying out humidity screen"));

			noInterrupts();

			tft.fillScreen(MonitorDisplay::BackgroundColor);

			tft.drawFastHLine(70, 165, 340, MonitorDisplay::LayoutLineColor);

			interrupts();
		}
	}

	void MonitorDisplay::LayoutTemperatureScreen()
	{
		if (this->regionChanged)
		{
			Serial.println(F("laying out temperature screen"));

			noInterrupts();

			tft.fillScreen(MonitorDisplay::BackgroundColor);

			tft.drawFastHLine(70, 165, 340, MonitorDisplay::LayoutLineColor);

			interrupts();
		}
	}

	void MonitorDisplay::LayoutSettingsScreen()
	{
		if (this->regionChanged)
		{
			Serial.println(F("laying out settings screen"));

			noInterrupts();

			tft.fillScreen(MonitorDisplay::BackgroundColor);

			tft.drawFastHLine(70, 165, 340, MonitorDisplay::LayoutLineColor);

			interrupts();
		}
	}

	void MonitorDisplay::LayoutMessageScreen()
	{
		if (this->regionChanged)
		{
			Serial.println(F("laying out message screen"));

			noInterrupts();

			tft.fillScreen(MonitorDisplay::BackgroundColor);

			tft.drawFastHLine(70, 165, 340, MonitorDisplay::LayoutLineColor);

			interrupts();
		}
	}

	void MonitorDisplay::DisplayHomeScreen()
	{
		this->LayoutHomeScreen();

		noInterrupts();

		// NOTE: each of the method calls here first over-write the previous value with the background, and 
		// then the current value in the defined color for readings.
		if (this->IntegerPartChanged(this->previousBME280Data.Temperature, this->currentBME280Data.Temperature) || this->regionChanged)
		{
			this->PrintTemperature(&this->previousBME280Data, MonitorDisplay::BackgroundColor);
			this->PrintTemperature(&this->currentBME280Data, MonitorDisplay::ReadingsTextColor);
		}

		if (this->IntegerPartChanged(this->previousBME280Data.Humidity, this->currentBME280Data.Humidity) || this->regionChanged)
		{
			this->PrintHumidity(&this->previousBME280Data, MonitorDisplay::BackgroundColor);
			this->PrintHumidity(&this->currentBME280Data, MonitorDisplay::ReadingsTextColor);
		}

		interrupts();

		this->previousBME280Data = this->currentBME280Data;
	}

	void MonitorDisplay::DisplayHumidityScreen()
	{
		this->LayoutHumidityScreen();

		noInterrupts();

		this->LoadMessage(F("Coming Soon!"));
		this->PrintMessage();

		interrupts();
	}

	void MonitorDisplay::DisplayTemperatureScreen()
	{
		this->LayoutTemperatureScreen();

		noInterrupts();

		this->LoadMessage(F("Coming Soon!"));
		this->PrintMessage();

		interrupts();
	}

	void MonitorDisplay::DisplaySettingsScreen()
	{
		this->LayoutSettingsScreen();

		int fm = freeMemory();

		noInterrupts();

		if (this->previousFreeMemory != fm || this->regionChanged)
		{
			this->PrintFreeMemory(fm);
		}

		interrupts();
	}

	void MonitorDisplay::DisplayMessageScreen()
	{
		this->LayoutMessageScreen();

		noInterrupts();

		this->PrintMessage();

		interrupts();
	}

	void MonitorDisplay::PrintHumidity(BME280Data* data, uint16_t color)
	{
		int8_t horizontalOffset = 7;
		tft.setFont(&bahnschrift90pt7b);
		tft.setTextSize(1);
		char* humidity = BME280Data::ConvertFloatToString(data->Humidity, 2, 0);
		int16_t centeredTextXPosition = GetCenteredPosition(humidity, humidityArea.x, humidityArea.y, humidityArea.width) - horizontalOffset;
		tft.setCursor(centeredTextXPosition, humidityArea.y + humidityArea.height - 35);
		tft.setTextColor(color, this->BackgroundColor); // apparently this doesn't work with custom fonts?
		tft.print(humidity);
	}

	void MonitorDisplay::PrintTemperature(BME280Data* data, uint16_t color)
	{
		int8_t horizontalOffset = 7;
		tft.setFont(&bahnschrift90pt7b);
		tft.setTextSize(1);
		char* temperature = BME280Data::ConvertFloatToString(data->Temperature, 2, 0);
		int16_t centeredTextXPosition = GetCenteredPosition(temperature, temperatureArea.x, temperatureArea.y, temperatureArea.width) - horizontalOffset;
		tft.setCursor(centeredTextXPosition, temperatureArea.y + temperatureArea.height - 35);
		tft.setTextColor(color, this->BackgroundColor); // apparently this doesn't work with custom fonts?
		tft.print(temperature);
	}

	/// <summary>Prints the amount of free memory in bytes to the screen.</summary>
	/// <param name="freeMemory">The amount of free memory to display, in bytes.</param>
	void MonitorDisplay::PrintFreeMemory(int freeMemory)
	{
		char* memoryLabel = "Free SRAM: ";

		noInterrupts();

		tft.setFont(&calibrib12pt7b);
		tft.setTextSize(1);

		// overwrite
		tft.setCursor(145, 160);
		tft.setTextColor(MonitorDisplay::BackgroundColor);
		tft.print(memoryLabel);
		tft.print(previousFreeMemory);

		// print the value
		tft.setCursor(145, 160);
		tft.setTextColor(MonitorDisplay::ReadingsTextColor);
		tft.print(memoryLabel);
		tft.print(freeMemory);

		interrupts();

		this->previousFreeMemory = freeMemory;
	}

	void MonitorDisplay::PrintMessage()
	{
		tft.setFont(&calibrib12pt7b);
		tft.setTextSize(1);

		int centeredLocation = GetCenteredPosition(this->previousMessage, 0, 160, 480);
		tft.setCursor(centeredLocation, 160);
		tft.setTextColor(MonitorDisplay::BackgroundColor);
		tft.print(this->previousMessage);

		centeredLocation = GetCenteredPosition(this->currentMessage, 0, 160, 480);
		tft.setCursor(centeredLocation, 160);
		tft.setTextColor(MonitorDisplay::ReadingsTextColor);
		tft.print(this->currentMessage);

		this->previousMessage = this->currentMessage;
	}

	bool MonitorDisplay::IntegerPartChanged(float first, float second)
	{
		/*double fractpart1, intpart1, fractpart2, intpart2;

		fractpart1 = modf(first, &intpart1);
		fractpart2 = modf(second, &intpart2);

		return intpart1 != intpart2;*/
		char* f = BME280Data::ConvertFloatToString(first, 2, 0);
		char* s = BME280Data::ConvertFloatToString(second, 2, 0);

		return strcmp(f, s) != 0;
	}

	int16_t MonitorDisplay::GetCenteredPosition(char* text, int16_t x, int16_t y, int16_t areaWidth)
	{
		// values used for centering text in display areas (rectangles)
		int16_t  x1, y1;
		uint16_t textWidth, textHeight;

		tft.getTextBounds(text, x, y, &x1, &y1, &textWidth, &textHeight);
		return ((areaWidth - textWidth) / 2) + x;
	}

	int16_t MonitorDisplay::GetCenteredPosition(const char* text, int16_t x, int16_t y, int16_t areaWidth)
	{
		// values used for centering text in display areas (rectangles)
		int16_t  x1, y1;
		uint16_t textWidth, textHeight;
		tft.getTextBounds(text, x, y, &x1, &y1, &textWidth, &textHeight);
		return ((areaWidth - textWidth) / 2) + x;
	}

	int16_t MonitorDisplay::GetCenteredPosition(const __FlashStringHelper* text, int16_t x, int16_t y, int16_t areaWidth)
	{
		// values used for centering text in display areas (rectangles)
		int16_t  x1, y1;
		uint16_t textWidth, textHeight;
		tft.getTextBounds(text, x, y, &x1, &y1, &textWidth, &textHeight);
		return ((areaWidth - textWidth) / 2) + x;
	}
}
