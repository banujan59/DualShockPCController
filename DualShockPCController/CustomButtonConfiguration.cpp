#include "CustomButtonConfiguration.h"

#include <filesystem>
#include <utility>
#include "OSHelper.h"

CustomButtonConfiguration::CustomButtonConfiguration(std::string buttonLayoutName) :
	m_mouseAccelerationFactor(20),
	m_rumbleSensitivity(0),
	m_buttonConfigurationName(std::move(buttonLayoutName)),
	m_customButtonSequenceModeEnabled(false),
	m_lightBarColor(255),
	m_lightBarFadeEnabled(false),
	m_lightBarMode(LightBarMode::SINGLE_COLOR)
{
}

void CustomButtonConfiguration::AddButtonDownMapping(int button, std::function<void()> function)
{
	m_cButtonDownFunctionMap[button] = function;
}

void CustomButtonConfiguration::AddShortButtonUpMapping(int button, std::function<void()> function)
{
	m_cShortButtonUpFunctionMap[button] = function;
}

void CustomButtonConfiguration::AddLongButtonUpMapping(int button, std::function<void()> function)
{
	m_cLongButtonUpFunctionMap[button] = function;
}

void CustomButtonConfiguration::AddButtonUpMapping(int button, std::function<void()> function)
{
	AddShortButtonUpMapping(button, function);
	AddLongButtonUpMapping(button, function);
}

void CustomButtonConfiguration::OnKeyDown(int buttons)
{
	if(buttons != DualShock4Buttons::NO_BUTTONS)
	{
		if (m_cButtonDownFunctionMap.find(buttons) != m_cButtonDownFunctionMap.end())
		{
			m_cButtonDownFunctionMap[buttons]();
		}
	}
}

void CustomButtonConfiguration::OnKeyUp(int buttons, int durationMS)
{
	if(!m_customButtonSequenceModeEnabled)
	{
		if (buttons == DualShock4Buttons::OPTIONS)
		{
			m_customButtonSequenceModeEnabled = true;
		}

		else if (buttons != DualShock4Buttons::NO_BUTTONS)
		{
			if (durationMS >= LONG_PRESS_DURATION_THRESHOLD_MS && m_cLongButtonUpFunctionMap.find(buttons) != m_cLongButtonUpFunctionMap.end())
			{
				m_cLongButtonUpFunctionMap[buttons]();
			}

			else if (m_cShortButtonUpFunctionMap.find(buttons) != m_cShortButtonUpFunctionMap.end())
			{
				m_cShortButtonUpFunctionMap[buttons]();
			}
		}
	}

	else if (buttons != DualShock4Buttons::NO_BUTTONS)
	{
		m_customButtonSequence.IterateNext(buttons);
	}
}

void CustomButtonConfiguration::ActivateCustomButtonSequence()
{
	if(m_customButtonSequenceModeEnabled)
	{
		m_customButtonSequenceModeEnabled = false;
		m_customButtonSequence.ExecuteCommandAtCurrentNode();
		m_customButtonSequence.ResetIterator();
	}
}

void CustomButtonConfiguration::CenterMouseToScreen()
{
	long width, height;
	OSHelper::GetScreenResolution(width, height);

	OSHelper::MousePosition position;
	position.x = width / 2;
	position.y = height / 2;
	OSHelper::SetNewMousePosition(position);
}

void CustomButtonConfiguration::UpdateMousePosWithJoySticks(float stickLX, float stickLY) const
{
	OSHelper::MousePosition oMousePosition = OSHelper::GetCurrentMousePosition();
	oMousePosition.x = oMousePosition.x + static_cast<long>(static_cast<float>(m_mouseAccelerationFactor) * stickLX);
	oMousePosition.y = oMousePosition.y - static_cast<long>(static_cast<float>(m_mouseAccelerationFactor) * stickLY);
	SetNewMousePosition(oMousePosition);
}

void CustomButtonConfiguration::UpdateMouseScrollWithJoySticks(float stickRX, float stickRY) const
{
	OSHelper::TriggerVerticalScroll(stickRY);
	OSHelper::TriggerHorizontalScroll(stickRX);
}

void CustomButtonConfiguration::UpdateMouseWIthGyro(float gyroX, float gyroY)
{
	OSHelper::MousePosition oMousePosition = OSHelper::GetCurrentMousePosition();

	oMousePosition.x -= static_cast<long>(gyroY);
	oMousePosition.y -= static_cast<long>(gyroX);

	// if we are out of window boundaries, stop
	long windowWidth, windowHeight;
	OSHelper::GetScreenResolution(windowWidth, windowHeight);

	if (oMousePosition.x >= 0 && oMousePosition.x <= windowWidth &&
		oMousePosition.y >= 0 && oMousePosition.y <= windowHeight)
	{
		SetNewMousePosition(oMousePosition);
	}
}

std::string CustomButtonConfiguration::GetButtonLayoutName() const
{
	return m_buttonConfigurationName;
}


int CustomButtonConfiguration::GetMouseAccelerationFactor() const
{
	return m_mouseAccelerationFactor;
}

void CustomButtonConfiguration::SetMouseAccelerationFactor(int newFactor)
{
	m_mouseAccelerationFactor = newFactor;
}

int CustomButtonConfiguration::GetRumbleSensitivity() const
{
	return m_rumbleSensitivity;
}

void CustomButtonConfiguration::SetRumbleSensitivity(const unsigned int& newSensitivity)
{
	m_rumbleSensitivity = newSensitivity;
}

void CustomButtonConfiguration::GetRumbleValueForDS(int& smallRuble, int& bigRumble, int& targetRumbleValue)
{
	switch(targetRumbleValue)
	{
	case 1:
		smallRuble = 1;
		bigRumble = 0;
		break;

	case 2:
		smallRuble = 80;
		bigRumble = 0;
		break;

	case 3:
		smallRuble = 0;
		bigRumble = 5;
		break;

	case 4:
		smallRuble = 0;
		bigRumble = 255;
		break;

	default:
		smallRuble = 0;
		bigRumble = 0;
		break;
	}
}

int CustomButtonConfiguration::GetLightBarColor()
{
	return m_lightBarColor;
}

void CustomButtonConfiguration::GetRGBLightBarColor(uint8_t& red, uint8_t& green, uint8_t& blue)
{
	red = (m_lightBarColor >> 16) & 0xFF;
	green = (m_lightBarColor >> 8) & 0xFF;
	blue = m_lightBarColor & 0xFF;
}

LightBarMode CustomButtonConfiguration::GetLightBarMode()
{
	return m_lightBarMode;
}

bool CustomButtonConfiguration::GetLightBarFadeEnabled()
{
	return m_lightBarFadeEnabled;;
}

void CustomButtonConfiguration::SetRGBLightBarColor(uint8_t& red, uint8_t& green, uint8_t& blue)
{
	m_lightBarColor = red << 16 | green << 8 | blue;
}

void CustomButtonConfiguration::SetLightBarMode(LightBarMode& lightBarMode, bool& fadeEnabled)
{
	m_lightBarFadeEnabled = fadeEnabled;
	m_lightBarMode = lightBarMode;
}


void CustomButtonConfiguration::GetAllCustomCommands(std::vector<std::string>& commandNames, std::vector<std::string>& buttonList,
                                                     std::vector<std::string>& actionType)
{
	m_customButtonSequence.GetAllCustomCommands(commandNames, buttonList, actionType);
}

bool CustomButtonConfiguration::AddNewCustomCommand(std::string& commmandName, std::vector<int>& buttonSequence,
                                                    CustomButtonSequence::ActionType& actionType,
                                                    std::vector<std::string>& actionTypeParameters)
{
	std::function<void()> functionToExecute;

	switch (actionType)
	{
	case CustomButtonSequence::ActionType::OPEN_FILE_OR_PROGRAM:
		functionToExecute = [actionTypeParameters]()
		{
			for (std::string filesToOpen : actionTypeParameters)
			{
				if (std::filesystem::exists(filesToOpen))
					OSHelper::ExecuteApplication(filesToOpen.c_str());
			}
		};
		break;
	}

	return m_customButtonSequence.AddCommand(commmandName, buttonSequence, functionToExecute, actionType);
}

void CustomButtonConfiguration::RemoveCustomCommand(std::string& commandName)
{
	m_customButtonSequence.DeleteCommand(commandName);
}

void CustomButtonConfiguration::GetDSButtonNames(std::map<int, std::string>& container)
{
	container.clear();
	container[DPAD_UP] = "DPAD UP";
	container[DPAD_DOWN] = "DPAD DOWN";
	container[DPAD_LEFT] = "DPAD LEFT";
	container[DPAD_RIGHT] = "DPAD RIGHT";
	container[OPTIONS] = "OPTIONS";
	container[SHARE] = "SHARE";
	container[L3] = "L3";
	container[R3] = "R3";
	container[L1] = "L1";
	container[R1] = "R1";
	container[L2] = "L2";
	container[R2] = "R2";
	container[X] = "X";
	container[CIRCLE] = "CIRCLE";
	container[SQUARE] = "SQUARE";
	container[TRIANGLE] = "TRIANGLE";
	container[PS_BUTTON] = "PS BUTTON";
	container[CENTER_TOUCH_BAR] = "CENTER TOUCH BAR";

	// Do not include DualShock4Buttons::NO_BUTTONS here.
	// The UI knows there are no buttons pressed when it cannot find the matching button in the container
}