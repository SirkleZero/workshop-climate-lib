#include "TFTDisplay.h"

using namespace Sensors;

namespace Display {
#pragma region constructor and initialization
	/// <summary>Initializes a new instance of the <see cref="TFTDisplay"/> class.</summary>
	TFTDisplay::TFTDisplay() :
		tft(TFT_CS, TFT_DC),
		ts(STMPE_CS),
#if defined(DISPLAY_35)
		humidityArea(240, 85, 240, 170, ScreenRegion::Home),
		temperatureArea(0, 85, 240, 170, ScreenRegion::Home),
		settingsButton(435, 0, 45, 45, ScreenRegion::Home),
		homeButton(0, 0, 45, 45, ScreenRegion(ScreenRegion::Humidity | ScreenRegion::Settings | ScreenRegion::Temperature)),
		centeredMessageBox(0, 160, 480, 10)
#endif
#if defined(DISPLAY_24)
		humidityArea(160, 50, 160, 125, ScreenRegion::Home),
		temperatureArea(0, 50, 160, 125, ScreenRegion::Home),
		settingsButton(275, 0, 45, 45, ScreenRegion::Home),
		homeButton(0, 0, 45, 45, ScreenRegion(ScreenRegion::Humidity | ScreenRegion::Settings | ScreenRegion::Temperature)),
		centeredMessageBox(0, 120, 360, 10)
#endif
	{}

	/// <summary>Executes initialization logic for the object.</summary>
	/// <returns>An <see cref="InitializationResult"/> that describes the result of initialization.</returns>
	InitializationResult TFTDisplay::Initialize()
	{
		InitializationResult result;

		tft.begin();

		noInterrupts();
		tft.setRotation(TFTDisplay::ScreenOrientation);
		interrupts();

		this->height = tft.height();
		this->width = tft.width();
		this->midHeight = this->height / 2;
		this->midWidth = this->width / 2;

		this->touchscreenExists = ts.begin();

		result.IsSuccessful = true;
		return result;
	}

#pragma endregion

	/// <summary>Clears all displayed content from the screen by wiping it with a black background.</summary>
	void TFTDisplay::Clear()
	{
		noInterrupts();
		tft.fillScreen(TFTDisplay::BackgroundColor);
		interrupts();
	}

	void TFTDisplay::Display()
	{
		// use the Touched() function to determine if a screen area has been pressed, and if so, where.
		this->Display(this->Touched());
	}

	void TFTDisplay::Display(ScreenRegion region)
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
					//Serial.println(F("ScreenRegion::Home"));
					this->DisplayHomeScreen();
					break;
				case ScreenRegion::Humidity:
					//Serial.println(F("ScreenRegion::Humidity"));
					this->DisplayHumidityScreen();
					break;
				case ScreenRegion::Settings:
					//Serial.println(F("ScreenRegion::Settings"));
					this->DisplaySettingsScreen();
					break;
				case ScreenRegion::StatusMessage:
					//Serial.println(F("ScreenRegion::StatusMessage"));
					this->DisplayMessageScreen();
					break;
				case ScreenRegion::Temperature:
					//Serial.println(F("ScreenRegion::Temperature"));
					this->DisplayTemperatureScreen();
					break;
				default:
					//Serial.println(F("default"));
					this->DisplayHomeScreen();
					break;
			}

			// configure the button areas per the region we've clicked on / screen we are displaying.
			this->ConfigureButtons(this->activeRegion);

			// once the display has been updated, reset the state variables that track change status
			this->DisplayUpdated();
		}
	}

	void TFTDisplay::ConfigureButtons(ScreenRegion region)
	{
		this->humidityArea.Enable(region);
		this->temperatureArea.Enable(region);
		this->settingsButton.Enable(region);
		this->homeButton.Enable(region);
	}

	bool TFTDisplay::DisplayUpdatable()
	{
		return this->dataChanged || this->regionChanged;
	}

	void TFTDisplay::DisplayUpdated()
	{
		this->dataChanged = false;
		this->regionChanged = false;
	}

	/// <summary></summary>
	ScreenRegion TFTDisplay::Touched()
	{
		if (!this->ts.bufferEmpty())
		{
			// Convert the point that was clicked to compensate for the screen orientation. While
			// the display knows it's orientation, the touchscreen does not.
			TS_Point p = this->ts.getPoint();

#if defined(DISPLAY_35)
			// NOTE: this 3.5" calculation is different than the one for the 2.4" display
			// not entirely sure why the larger display version behaves differently than the
			// smaller version.
			int y = map(p.x, TFTDisplay::TSMaxY, TFTDisplay::TSMinY, 0, tft.height());
			int x = map(p.y, TFTDisplay::TSMinX, TFTDisplay::TSMaxX, 0, tft.width());
#endif
#if defined(DISPLAY_24)
			int y = map(p.x, TFTDisplay::TSMinY, TFTDisplay::TSMaxY, 0, tft.height());
			int x = map(p.y, TFTDisplay::TSMinX, TFTDisplay::TSMaxX, 0, tft.width());
#endif

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

	void TFTDisplay::LoadMessage(const __FlashStringHelper* message)
	{
		this->currentMessage = message;
		this->dataChanged = true;
	}

	/// <summary>Prints sensor information to the screen.</summary>
	/// <param name="data">The <see cref="BME280Data"> containing readings from the sensors.</param>
	void TFTDisplay::LoadSensorData(BME280Data data)
	{
		this->currentBME280Data = data;
		this->dataChanged = true;
	}

	/// <summary>Draws the initial static elements of the display.</summary>
	void TFTDisplay::LayoutHomeScreen()
	{
		if (this->regionChanged)
		{
			noInterrupts();

			tft.fillScreen(TFTDisplay::BackgroundColor);

			// for centering
			int16_t centeredTextXPosition;

			// draw our centerline
#if defined(DISPLAY_35)
			tft.drawFastVLine(240, 30, 260, TFTDisplay::LayoutLineColor);
#endif
#if defined(DISPLAY_24)
			tft.drawFastVLine(160, 30, 180, TFTDisplay::LayoutLineColor);
#endif
			// set font
			tft.setFont(this->mainReadingsLabelFont);
			tft.setTextColor(TFTDisplay::LayoutTextColor);
			tft.setTextSize(1);

			// for temperature
			//char* temperatureLabel = "Fahrenheit";
			const char* temperatureLabel = "Fahrenheit";
			//tft.drawRect(temperatureArea.x, temperatureArea.y, temperatureArea.width, temperatureArea.height, TFTDisplay::LayoutLineColor);
			centeredTextXPosition = GetCenteredPosition(temperatureLabel, temperatureArea.x, temperatureArea.y, temperatureArea.width);
			tft.setCursor(centeredTextXPosition, temperatureArea.y + temperatureArea.height - 5);
			tft.println(temperatureLabel);

			// for Humidity
			//char* humidityLabel = "% Humidity";
			const char* humidityLabel = "% Humidity";
			//tft.drawRect(humidityArea.x, humidityArea.y, humidityArea.width, humidityArea.height, TFTDisplay::LayoutLineColor);
			centeredTextXPosition = GetCenteredPosition(humidityLabel, humidityArea.x, humidityArea.y, humidityArea.width);
			tft.setCursor(centeredTextXPosition, humidityArea.y + humidityArea.height - 5);
			tft.println(humidityLabel);

			// draw the box for the settings screen
			//tft.drawRect(settingsButton.x, settingsButton.y, settingsButton.width, settingsButton.height, TFTDisplay::LayoutLineColor);

			// draw the box for the home screen
			//tft.drawRect(homeButton.x, homeButton.y, homeButton.width, homeButton.height, TFTDisplay::LayoutLineColor);

			interrupts();
		}
	}

	void TFTDisplay::LayoutHumidityScreen()
	{
		if (this->regionChanged)
		{
			noInterrupts();

			tft.fillScreen(TFTDisplay::BackgroundColor);

#if defined(DISPLAY_35)
			tft.drawFastHLine(70, 165, 340, TFTDisplay::LayoutLineColor);
#endif
#if defined(DISPLAY_24)
			tft.drawFastHLine(70, 123, 177, TFTDisplay::LayoutLineColor);
#endif
			
			interrupts();
		}
	}

	void TFTDisplay::LayoutTemperatureScreen()
	{
		if (this->regionChanged)
		{
			noInterrupts();

			tft.fillScreen(TFTDisplay::BackgroundColor);

#if defined(DISPLAY_35)
			tft.drawFastHLine(70, 165, 340, TFTDisplay::LayoutLineColor);
#endif
#if defined(DISPLAY_24)
			tft.drawFastHLine(70, 123, 177, TFTDisplay::LayoutLineColor);
#endif
			
			interrupts();
		}
	}

	void TFTDisplay::LayoutSettingsScreen()
	{
		if (this->regionChanged)
		{
			noInterrupts();

			tft.fillScreen(TFTDisplay::BackgroundColor);

#if defined(DISPLAY_35)
			tft.drawFastHLine(70, 165, 340, TFTDisplay::LayoutLineColor);
#endif
#if defined(DISPLAY_24)
			tft.drawFastHLine(70, 123, 177, TFTDisplay::LayoutLineColor);
#endif
			interrupts();
		}
	}

	void TFTDisplay::LayoutMessageScreen()
	{
		if (this->regionChanged)
		{
			noInterrupts();

			tft.fillScreen(TFTDisplay::BackgroundColor);

#if defined(DISPLAY_35)
			tft.drawFastHLine(70, 165, 340, TFTDisplay::LayoutLineColor);
#endif
#if defined(DISPLAY_24)
			tft.drawFastHLine(70, 123, 177, TFTDisplay::LayoutLineColor);
#endif

			interrupts();
		}
	}

	void TFTDisplay::DisplayHomeScreen()
	{
		this->LayoutHomeScreen();

		noInterrupts();

		// NOTE: each of the method calls here first over-write the previous value with the background, and 
		// then the current value in the defined color for readings.
		if (this->IntegerPartChanged(this->previousBME280Data.Temperature, this->currentBME280Data.Temperature) || this->regionChanged)
		{
			this->PrintTemperature(&this->previousBME280Data, TFTDisplay::BackgroundColor);
			this->PrintTemperature(&this->currentBME280Data, TFTDisplay::ReadingsTextColor);
		}

		if (this->IntegerPartChanged(this->previousBME280Data.Humidity, this->currentBME280Data.Humidity) || this->regionChanged)
		{
			this->PrintHumidity(&this->previousBME280Data, TFTDisplay::BackgroundColor);
			this->PrintHumidity(&this->currentBME280Data, TFTDisplay::ReadingsTextColor);
		}

		interrupts();

		this->previousBME280Data = this->currentBME280Data;
	}

	void TFTDisplay::DisplayHumidityScreen()
	{
		this->LayoutHumidityScreen();

		noInterrupts();

		this->LoadMessage(F("Coming Soon!"));
		this->PrintMessage();

		interrupts();
	}

	void TFTDisplay::DisplayTemperatureScreen()
	{
		this->LayoutTemperatureScreen();

		noInterrupts();

		this->LoadMessage(F("Coming Soon!"));
		this->PrintMessage();

		interrupts();
	}

	void TFTDisplay::DisplaySettingsScreen()
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

	void TFTDisplay::DisplayMessageScreen()
	{
		this->LayoutMessageScreen();

		noInterrupts();

		this->PrintMessage();

		interrupts();
	}

	void TFTDisplay::PrintHumidity(BME280Data* data, uint16_t color)
	{
		int8_t horizontalOffset = 7;
		tft.setFont(this->mainReadingsFont);
		tft.setTextSize(1);
		static char humidity[15];
		BME280Data::ConvertFloatToString(data->Humidity, 2, 0, humidity);
		int16_t centeredTextXPosition = GetCenteredPosition(humidity, humidityArea.x, humidityArea.y, humidityArea.width) - horizontalOffset;
		tft.setCursor(centeredTextXPosition, humidityArea.y + humidityArea.height - 35);
		tft.setTextColor(color, this->BackgroundColor); // apparently this doesn't work with custom fonts?
		tft.print(humidity);
	}

	void TFTDisplay::PrintTemperature(BME280Data* data, uint16_t color)
	{
		int8_t horizontalOffset = 7;
		tft.setFont(this->mainReadingsFont);
		tft.setTextSize(1);
		static char temperature[15];
		BME280Data::ConvertFloatToString(data->Temperature, 2, 0, temperature);
		int16_t centeredTextXPosition = GetCenteredPosition(temperature, temperatureArea.x, temperatureArea.y, temperatureArea.width) - horizontalOffset;
		tft.setCursor(centeredTextXPosition, temperatureArea.y + temperatureArea.height - 35);
		tft.setTextColor(color, this->BackgroundColor); // apparently this doesn't work with custom fonts?
		tft.print(temperature);
	}

	/// <summary>Prints the amount of free memory in bytes to the screen.</summary>
	/// <param name="freeMemory">The amount of free memory to display, in bytes.</param>
	void TFTDisplay::PrintFreeMemory(int freeMemory)
	{
		noInterrupts();

		tft.setFont(this->statusMessageFont);
		tft.setTextSize(1);

		char* memoryLabel = "Free SRAM: ";

		// overwrite
		tft.setCursor(this->midWidth / 2, this->midHeight);
		tft.setTextColor(TFTDisplay::BackgroundColor);
		tft.print(memoryLabel);
		tft.print(previousFreeMemory);

		// print the value
		tft.setCursor(this->midWidth / 2, this->midHeight);
		tft.setTextColor(TFTDisplay::ReadingsTextColor);
		tft.print(memoryLabel);
		tft.print(freeMemory);

		interrupts();

		this->previousFreeMemory = freeMemory;
	}

	void TFTDisplay::PrintMessage()
	{
		tft.setFont(this->statusMessageFont);
		tft.setTextSize(1);

		int centeredLocation = GetCenteredPosition(this->previousMessage, 0, this->midHeight, this->width);
		tft.setCursor(centeredLocation, this->midHeight);
		tft.setTextColor(TFTDisplay::BackgroundColor);
		tft.print(this->previousMessage);

		centeredLocation = GetCenteredPosition(this->currentMessage, 0, this->midHeight, this->width);
		tft.setCursor(centeredLocation, this->midHeight);
		tft.setTextColor(TFTDisplay::ReadingsTextColor);
		tft.print(this->currentMessage);

		this->previousMessage = this->currentMessage;
	}

	bool TFTDisplay::IntegerPartChanged(float first, float second)
	{
		static char f[15];
		BME280Data::ConvertFloatToString(first, 2, 0, f);

		static char s[15];
		BME280Data::ConvertFloatToString(second, 2, 0, s);

		return strcmp(f, s) != 0;
	}

	int16_t TFTDisplay::GetCenteredPosition(char* text, int16_t x, int16_t y, int16_t areaWidth)
	{
		// values used for centering text in display areas (rectangles)
		int16_t  x1, y1;
		uint16_t textWidth, textHeight;

		tft.getTextBounds(text, x, y, &x1, &y1, &textWidth, &textHeight);
		return ((areaWidth - textWidth) / 2) + x;
	}

	int16_t TFTDisplay::GetCenteredPosition(const char* text, int16_t x, int16_t y, int16_t areaWidth)
	{
		// values used for centering text in display areas (rectangles)
		int16_t  x1, y1;
		uint16_t textWidth, textHeight;
		tft.getTextBounds(text, x, y, &x1, &y1, &textWidth, &textHeight);
		return ((areaWidth - textWidth) / 2) + x;
	}

	int16_t TFTDisplay::GetCenteredPosition(const __FlashStringHelper* text, int16_t x, int16_t y, int16_t areaWidth)
	{
		// values used for centering text in display areas (rectangles)
		int16_t  x1, y1;
		uint16_t textWidth, textHeight;
		tft.getTextBounds(text, x, y, &x1, &y1, &textWidth, &textHeight);
		return ((areaWidth - textWidth) / 2) + x;
	}
}
