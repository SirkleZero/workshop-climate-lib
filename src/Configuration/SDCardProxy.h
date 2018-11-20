#ifndef SDCardProxy_h
#define SDCardProxy_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <SPI.h>

#include "ControllerConfiguration.h"
#include "Secrets.h"

namespace Configuration {
	/// <summary>Provides a class that wraps reading an SD Card.</summary>
	class SDCardProxy {
	public:		
		SDCardProxy();
		
		void Initialize();		
		bool LoadSecrets(Secrets *secrets);		
		bool LoadConfiguration(ControllerConfiguration *configuration);
		void PrintDebug();
	private:
		/// <summary>The name and path of the secrets file.</summary>
		const char *SecretsFileName = "SECRET~1.JSO";

		/// <summary>The name and path of the configuration file.</summary>
		const char *ControllerConfigurationFileName = "CONTRO~1.JSO";

		/// <summary>The chip select pin used by the SD card.</summary>
		const uint8_t SDCardChipSelectPin = 5;
	};
}
#endif
