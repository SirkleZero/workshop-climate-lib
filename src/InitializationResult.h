#pragma once

#include <Arduino.h>

///<summary>Represents the data that describes the result of an initialization function.</summary>
struct InitializationResult {
	/// <summary>Initializes a new instance of the <see cref="InitializationResult"/> struct.</summary>
	InitializationResult() {}

	///<summary>true if the initialization was successful; otherwise false.</summary>
	bool IsSuccessful = false;
	/// <summary>The string containing any error message that occurred.</summary>
	const __FlashStringHelper *ErrorMessage;

	///<summary>Prints a debug statement to Serial output.</summary>
	virtual void PrintDebug()
	{
		Serial.println(F("---------------------------------------"));
		Serial.print(F("Is Successful: ")); Serial.println(this->IsSuccessful);
		Serial.print(F("Error Message: ")); Serial.println(this->ErrorMessage);
		Serial.println(F("---------------------------------------"));
		Serial.println();
	}
};
