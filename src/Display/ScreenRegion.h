#pragma once

namespace Display {
	/// <summary>Specifies the touch screen regions of the system.</summary>
	enum ScreenRegion
	{
		/// <summary>Represents the screen not being touched in a defined region.</summary>
		None = 1u << 0,
		/// <summary>Represents that the Home region has been touched.</summary>
		Home = 1u << 1,
		/// <summary>Represents that the Settings region has been touched.</summary>
		Settings = 1u << 2,
		/// <summary>Represents that the Humidity region has been touched.</summary>
		Humidity = 1u << 3,
		/// <summary>Represents that the Temperature region has been touched.</summary>
		Temperature = 1u << 4,
		/// <summary>Represents that the screen should show a status message of some kind.</summary>
		StatusMessage = 1u << 5
	};
}
