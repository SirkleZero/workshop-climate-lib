#include "SDCardProxy.h"

namespace Configuration {
	/// <summary>Initializes a new instance of the <see cref="SDCardProxy"/> class.</summary>
	SDCardProxy::SDCardProxy() {}

	/// <summary>Executes initialization logic for the object.</summary>
	/// <returns>An <see cref="InitializationResult"/> that describes the result of initialization.</returns>
	InitializationResult SDCardProxy::Initialize()
	{
		InitializationResult result;

		if (!SD.begin(SDCardProxy::SDCardChipSelectPin))
		{
			result.ErrorMessage = F("Failed to initialize the SD card.");
			return result;
		}

		result.IsSuccessful = true;
		return result;
	}

	/// <summary>Loads the secrets from the SD Card.</summary>
	/// <param name=""></param>
	/// <remarks>The name of the secrets file is: SECRET~1.JSO</remarks>
	bool SDCardProxy::LoadSecrets(Secrets *secrets)
	{
		// Open file for reading
		File file = SD.open(SecretsFileName);

		// Allocate the memory pool on the stack.
		// Don't forget to change the capacity to match your JSON document.
		// Use arduinojson.org/assistant to compute the capacity.
		StaticJsonBuffer<512> jsonBuffer;

		// Parse the root object
		JsonObject &root = jsonBuffer.parseObject(file);

		if (!root.success())
		{
			Serial.println(F("Failed to read file"));
			file.close();
			return false;
		}

		// Read the data from the json config file. In this case make sure
		// we are using the set methods to copy our const char * values that
		// come from the stream.
		secrets->SetAdafruitIOUsername(root["adafruitio_username"]);
		secrets->SetAdafruitIOAccessKey(root["adafruitio_key"]);
		secrets->SetWifiSSID(root["wifi_ssid"]);
		secrets->SetWifiPassword(root["wifi_password"]);

		// Close the file (File's destructor doesn't close the file)
		file.close();
		return true;
	}

	/// <summary>Loads the configuration from the SD Card.</summary>
	/// <param name=""></param>
	/// <remarks>The name of the configuration file is: CONTRO~1.JSO</remarks>
	bool SDCardProxy::LoadConfiguration(ControllerConfiguration *configuration)
	{
		// Open file for reading
		File file = SD.open(ControllerConfigurationFileName);

		// Allocate the memory pool on the stack.
		// Don't forget to change the capacity to match your JSON document.
		// Use arduinojson.org/assistant to compute the capacity.
		StaticJsonBuffer<512> jsonBuffer;

		// Parse the root object
		JsonObject &root = jsonBuffer.parseObject(file);

		if (!root.success())
		{
			Serial.println(F("Failed to read file"));
			file.close();
			return false;
		}

		// Read the values from the config file. None of these are strings, 
		// so we don't have to worry about making copies of the data into 
		// the configuration object.
		configuration->RunawayTimeLimit = root["RunawayTimeLimit"];
		configuration->MinimumHumidity = root["MinimumHumidity"];
		configuration->TargetHumidity = root["TargetHumidity"];
		configuration->MaximumHumidity = root["MaximumHumidity"];
		configuration->HumidifierOperationOffset = root["HumidifierOperationOffset"];
		configuration->DeHumidifierOperationOffset = root["DeHumidifierOperationOffset"];

		// Close the file (File's destructor doesn't close the file)
		file.close();
		return true;
	}

	void SDCardProxy::LogMessage(const __FlashStringHelper *message)
	{
		File logfile = SD.open(SDCardProxy::LogFileName, FILE_WRITE);		
		if (!SD.exists(SDCardProxy::LogFileName))
		{
			// write out a header for the file.
			logfile.print(F("millis\t")); logfile.println(F("message"));
		}

		logfile.print(millis()); logfile.print("\t");  logfile.println(message);

		logfile.flush();
		logfile.close();
	}

	/// <summary>Prints a debug statement to Serial output.</summary>
	void SDCardProxy::PrintDebug()
	{
		// Open file for reading
		File file = SD.open(ControllerConfigurationFileName);
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
