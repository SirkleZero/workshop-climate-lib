#ifndef SDCardProxy_h
#define SDCardProxy_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <SPI.h>

#include "ControllerConfiguration.h"
#include "Secrets.h"

namespace Configuration {
	class SDCardProxy {
	public:
		SDCardProxy();
		void Initialize();
		void LoadSecrets(Secrets *secrets);
		void LoadConfiguration(ControllerConfiguration *configuration);

		void PrintDebug();
	private:
		const char *secretsFileName = "/SECRET~1.JSO";  // <- SD library uses 8.3 filenames
		const char *controllerConfigurationFileName = "CONTRO~1.JSO";
	};
}
#endif
