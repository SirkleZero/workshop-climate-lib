#ifndef ControllerConfiguration_h
#define ControllerConfiguration_h

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
		/// <summary>Specifies the offset value of the humidity sensor to ensure proper calibration and accuracy.</summary>
		float HumidityOffset;

		/// <summary>Prints a debug statement to Serial output.</summary>
		void PrintDebug()
		{
			Serial.println(F("---------------------------------------"));
			Serial.print(F("Runaway Time Limit = ")); Serial.println(this->RunawayTimeLimit);
			Serial.print(F("Minimum Humidity = ")); Serial.println(this->MinimumHumidity);
			Serial.print(F("Target Humidity = ")); Serial.println(this->TargetHumidity);
			Serial.print(F("Maximum Humidity = ")); Serial.println(this->MaximumHumidity);
			Serial.print(F("Humidity Offset = ")); Serial.println(this->HumidityOffset);
			Serial.println(F("---------------------------------------"));
			Serial.println();
		}
	};
}
#endif
