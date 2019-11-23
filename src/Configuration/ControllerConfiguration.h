#pragma once

#include <Arduino.h>

namespace Configuration {
	/// <summary>Provides a class that holds configuration information used for system processing.</summary>
	class ControllerConfiguration {
	private:

	public:
		/// <summary>Specifies the duration in milliseconds that we allow the system to run without data before we perform an emergency shut down.</summary>
		unsigned long RunawayTimeLimit;
		/// <summary>Specifies the minimum humidity allowed.</summary>
		float MinimumHumidity;
		/// <summary>Specifies the ideal humidity for the system.</summary>
		float TargetHumidity;
		/// <summary>Specifies the maximum humidity allowed.</summary>
		float MaximumHumidity;
		/// <summary>Specifies the offset to be used to compensate for humidification over/under run.</summary>
		float HumidifierOperationOffset;
		/// <summary>Specifies the offset to be used to compensate for de-humidification over/under run.</summary>
		float DeHumidifierOperationOffset;
		/// <summary>Specifies how often in milliseconds the controller will poll its sensors for readings.</summary>
		float PollIntervalMS;

		/// <summary>Prints a debug statement to Serial output.</summary>
		void PrintDebug()
		{
			Serial.println(F("---------------------------------------"));
			Serial.print(F("Runaway Time Limit = ")); Serial.println(this->RunawayTimeLimit);
			Serial.print(F("Minimum Humidity = ")); Serial.println(this->MinimumHumidity);
			Serial.print(F("Target Humidity = ")); Serial.println(this->TargetHumidity);
			Serial.print(F("Maximum Humidity = ")); Serial.println(this->MaximumHumidity);
			Serial.print(F("Humidifier Operation Offset = ")); Serial.println(this->HumidifierOperationOffset);
			Serial.print(F("De-Humidifier Operation Offset = ")); Serial.println(this->DeHumidifierOperationOffset);
			Serial.print(F("Poll Interval = ")); Serial.println(this->PollIntervalMS);
			Serial.println(F("---------------------------------------"));
			Serial.println();
		}
	};
}
