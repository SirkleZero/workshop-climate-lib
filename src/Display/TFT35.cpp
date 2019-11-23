#include "TFT35.h"

using namespace Sensors;

namespace Display {
#pragma region constructor and initialization

	/// <summary>Initializes a new instance of the <see cref="35Display"/> class.</summary>
	TFT35::TFT35() : TFTDisplay()
	{
		//Adafruit_HX8357 t = Adafruit_HX8357(TFT_CS, TFT_DC);
		//this->tft = &t;
		
		this->humidityArea = ButtonRectangle(240, 85, 240, 170, ScreenRegion::Home);
		this->temperatureArea = ButtonRectangle(0, 85, 240, 170, ScreenRegion::Home);
		this->settingsButton = ButtonRectangle(435, 0, 45, 45, ScreenRegion::Home);
		this->homeButton = ButtonRectangle(0, 0, 45, 45, ScreenRegion(ScreenRegion::Humidity | ScreenRegion::Settings | ScreenRegion::Temperature));
		this->centeredMessageBox = Rectangle(0, 160, 480, 10);
	}

#pragma endregion
}
