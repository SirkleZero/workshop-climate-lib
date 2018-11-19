#include "SDCardProxy.h"

namespace Configuration {
	SDCardProxy::SDCardProxy() {}

	void SDCardProxy::Initialize()
	{
		while (!SD.begin(5))
		{
			Serial.println(F("Failed to initialize SD library"));
			delay(1000);
		}
	}

	void SDCardProxy::LoadSecrets(Secrets *secrets)
	{
		// Open file for reading
		File file = SD.open(secretsFileName);

		// Allocate the memory pool on the stack.
		// Don't forget to change the capacity to match your JSON document.
		// Use arduinojson.org/assistant to compute the capacity.
		StaticJsonBuffer<512> jsonBuffer;

		// Parse the root object
		JsonObject &root = jsonBuffer.parseObject(file);

		if (!root.success())
		{
			Serial.println(F("Failed to read file"));
		}

		secrets->SetUsername(root["adafruitio_username"]);
		secrets->SetPassword(root["adafruitio_key"]);
		secrets->SetWifiSSID(root["wifi_ssid"]);
		secrets->SetWifiPassword(root["wifi_password"]);

		// Close the file (File's destructor doesn't close the file)
		file.close();
	}

	void SDCardProxy::LoadConfiguration(ControllerConfiguration *configuration)
	{
		// Open file for reading
		File file = SD.open(controllerConfigurationFileName);

		// Allocate the memory pool on the stack.
		// Don't forget to change the capacity to match your JSON document.
		// Use arduinojson.org/assistant to compute the capacity.
		StaticJsonBuffer<512> jsonBuffer;

		// Parse the root object
		JsonObject &root = jsonBuffer.parseObject(file);

		if (!root.success())
		{
			Serial.println(F("Failed to read file"));
		}

		configuration->RunawayTimeLimit = root["RunawayTimeLimit"];
		configuration->MinimumHumidity = root["MinimumHumidity"];
		configuration->TargetHumidity = root["TargetHumidity"];
		configuration->MaximumHumidity = root["MaximumHumidity"];
		configuration->HumidityOffset = root["HumidityOffset"];

		// Close the file (File's destructor doesn't close the file)
		file.close();
	}

	// Prints the content of a file to the Serial
	void SDCardProxy::PrintDebug()
	{
		// Open file for reading
		File file = SD.open(controllerConfigurationFileName);
		if (!file)
		{
			Serial.println(F("Failed to read file"));
			return;
		}

		// Extract each characters by one by one
		while (file.available())
		{
			Serial.print((char)file.read());
		}
		Serial.println();

		// Close the file (File's destructor doesn't close the file)
		file.close();
	}
}
