#ifndef SDCardProxy_h
#define SDCardProxy_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <SD.h>

#include "..\InitializationResult.h"
#include "ControllerConfiguration.h"
#include "Secrets.h"

namespace Configuration {
	/// <summary>Provides a class that wraps reading an SD Card.</summary>
	class SDCardProxy {
	public:		
		SDCardProxy();
		
		InitializationResult Initialize();
		bool LoadSecrets(Secrets *secrets);		
		bool LoadConfiguration(ControllerConfiguration *configuration);
		void LogMessage(const __FlashStringHelper *message);
		void PrintDebug();
	private:
		/// <summary>The chip select pin used by the SD card.</summary>
		static const uint8_t SDCardChipSelectPin = 5;

		/// <summary>The name and path of the secrets file.</summary>
		const char *SecretsFileName = "SECRET~1.JSO";

		/// <summary>The name and path of the configuration file.</summary>
		const char *ControllerConfigurationFileName = "CONTRO~1.JSO";

		/// <summary>The name and path of the log file.</summary>
		const char *LogFileName = "log.txt";
	};
}
#endif
