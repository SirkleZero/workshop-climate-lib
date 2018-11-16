#include "HumidityRelayManager.h"

using namespace Sensors;

namespace Relay {
	HumidityRelayManager::HumidityRelayManager() :
		Orange(0xe6, 0x5c, 0x00),
		Green(0x33, 0xcc, 0x33),
		Purple(0x66, 0x00, 0xcc),
		Aqua(0x00, 0x99, 0xff),
		Red(0x99, 0x00, 0x00)
	{}

	void HumidityRelayManager::Initialize(ControllerConfiguration *configuration)
	{
		this->configuration = configuration;

		// relay control pins
		pinMode(HumidityRelayManager::HumidifierControlPin, OUTPUT);
		pinMode(HumidityRelayManager::DehumidifierControlPin, OUTPUT);

		this->EnableIndicator();

		this->SetIndicatorColor(HumidityRelayManager::Orange);
	}

	void HumidityRelayManager::KeepAlive()
	{
		this->previousKeepAliveCall = millis();
	}

	void HumidityRelayManager::AdjustClimate(SensorData data)
	{
		this->KeepAlive();

		// check to see if we are either humidifying or dehumidifying
		if (this->humidificationState == HumidificationState::Humidifying || this->humidificationState == HumidificationState::Dehumidifying)
		{
			// if we're either humidifying or dehumidifying, then we need to determine if we should continue, or if we've hit our target humidity.
			switch (this->humidificationState)
			{
				case HumidificationState::Dehumidifying:
					// we are dehumidifying, check against the target level and run or
					// shut down appropriately
					if (data.climate.Humidity <= this->configuration->TargetHumidity)
					{
						// we hit our target, shut down so we don't overshoot by too much!
						this->ShutDown();
					}
					break;
				case HumidificationState::Humidifying:
					// we are humidifying, check against the target level and run or
					// shut down appropriately
					if (data.climate.Humidity >= this->configuration->TargetHumidity)
					{
						// we hit our target, shut down so we don't overshoot by too much!
						this->ShutDown();
					}
					break;
			}
		}
		else
		{
			// we are neither humidifying or dehumidifying, so let's check to see if we are outside of our min and max operating parameters. If we are in the range, then we're goldilocks zoned, otherwise, we need to turn on either the humdififier or the dehumidifier.
			// figure out which mode we need to be in. Humidification or Dehumidification.
			if (data.climate.Humidity > this->configuration->MaximumHumidity)
			{
				// it's too humid, enable the dehumidifier
				this->EnableDehumidifier();
			}
			else if (data.climate.Humidity < this->configuration->MinimumHumidity)
			{
				// it's too dry, enable the humidifier
				this->EnableHumidifier();
			}
			else
			{
				// goldilocks zone, shut them both down
				this->ShutDown();
			}
		}

		//switch (this->humidificationState)
		//{
		//	case HumidificationState::Dehumidifying:
		//		// we are dehumidifying, check against the target level and run or
		//		// shut down appropriately
		//		if (data.climate.Humidity <= this->configuration->TargetHumidity)
		//		{
		//			// we hit our target, shut down so we don't overshoot by too much!
		//			this->ShutDown();
		//		}

		//		break;
		//	case HumidificationState::Humidifying:
		//		// we are humidifying, check against the target level and run or
		//		// shut down appropriately
		//		if (data.climate.Humidity >= this->configuration->TargetHumidity)
		//		{
		//			// we hit our target, shut down so we don't overshoot by too much!
		//			this->ShutDown();
		//		}

		//		break;
		//	case HumidificationState::None:
		//		// figure out which mode we need to be in. Humidification or Dehumidification.
		//		if (data.climate.Humidity > this->configuration->MaximumHumidity)
		//		{
		//			// it's too humid, enable the dehumidifier
		//			this->EnableDehumidifier();
		//		}
		//		else if (data.climate.Humidity < this->configuration->MinimumHumidity)
		//		{
		//			// it's too dry, enable the humidifier
		//			this->EnableHumidifier();
		//		}
		//		else
		//		{
		//			// goldilocks zone, shut them both down
		//			this->ShutDown();
		//		}

		//		break;
		//}
	}

	void HumidityRelayManager::EmergencyShutoff()
	{
		currentMillis = millis();
		if (this->currentMillis - this->previousKeepAliveCall >= this->configuration->RunawayTimeLimit)
		{
			// we exceeded our time! Shut it down!
			this->ShutDown();
			this->SetIndicatorColor(HumidityRelayManager::Purple);
		}
	}

	void HumidityRelayManager::EnableHumidifier()
	{
		this->DisableDehumidifier();
		this->humidificationState = HumidificationState::Humidifying;
		this->SetIndicatorColor(HumidityRelayManager::Aqua);

		digitalWrite(HumidityRelayManager::HumidifierControlPin, HIGH);
	}

	void HumidityRelayManager::EnableDehumidifier()
	{
		this->DisableHumidifier();
		this->humidificationState = HumidificationState::Dehumidifying;
		this->SetIndicatorColor(HumidityRelayManager::Red);

		digitalWrite(HumidityRelayManager::DehumidifierControlPin, HIGH);
	}

	void HumidityRelayManager::DisableHumidifier()
	{
		digitalWrite(HumidityRelayManager::HumidifierControlPin, LOW);
	}

	void HumidityRelayManager::DisableDehumidifier()
	{
		digitalWrite(HumidityRelayManager::DehumidifierControlPin, LOW);
	}

	void HumidityRelayManager::ShutDown()
	{
		this->humidificationState = HumidificationState::None;
		this->SetIndicatorColor(HumidityRelayManager::Green);

		this->DisableHumidifier();
		this->DisableDehumidifier();
	}

	void HumidityRelayManager::SetIndicatorColor(RGB color)
	{
		this->EnableIndicator();

		if (HumidityRelayManager::CommonAnode)
		{
			analogWrite(HumidityRelayManager::RedPin, 255 - color.Red);
			analogWrite(HumidityRelayManager::GreenPin, 255 - color.Green);
			analogWrite(HumidityRelayManager::BluePin, 255 - color.Blue);
		}
		else
		{
			analogWrite(HumidityRelayManager::RedPin, color.Red);
			analogWrite(HumidityRelayManager::GreenPin, color.Green);
			analogWrite(HumidityRelayManager::BluePin, color.Blue);
		}
	}

	void HumidityRelayManager::DisableIndicator()
	{
		if (this->indicatorEnabled)
		{
			pinMode(HumidityRelayManager::RedPin, INPUT);
			pinMode(HumidityRelayManager::GreenPin, INPUT);
			pinMode(HumidityRelayManager::BluePin, INPUT);

			this->indicatorEnabled = false;
		}
	}

	void HumidityRelayManager::EnableIndicator()
	{
		if (!this->indicatorEnabled)
		{
			pinMode(HumidityRelayManager::RedPin, OUTPUT);
			pinMode(HumidityRelayManager::GreenPin, OUTPUT);
			pinMode(HumidityRelayManager::BluePin, OUTPUT);

			this->indicatorEnabled = true;
		}
	}
}
