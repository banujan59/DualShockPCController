#include "CustomButtonHandler.h"

void CustomButtonHandler::AddButtonDownMapping(int button, std::function<void()> function)
{
	m_cButtonDownFunctionMap[button] = function;
}

void CustomButtonHandler::AddShortButtonUpMapping(int button, std::function<void()> function)
{
	m_cShortButtonUpFunctionMap[button] = function;
}

void CustomButtonHandler::AddLongButtonUpMapping(int button, std::function<void()> function)
{
	m_cLongButtonUpFunctionMap[button] = function;
}

void CustomButtonHandler::AddButtonUpMapping(int button, std::function<void()> function)
{
	AddShortButtonUpMapping(button, function);
	AddLongButtonUpMapping(button, function);
}

void CustomButtonHandler::OnKeyDown(int buttons)
{
	if(buttons != DualShock4Buttons::NO_BUTTONS)
	{
		if (m_cButtonDownFunctionMap.find(buttons) != m_cButtonDownFunctionMap.end())
		{
			m_cButtonDownFunctionMap[buttons]();
		}
	}
}

void CustomButtonHandler::OnKeyUp(int buttons, int durationMS)
{
	if(buttons != DualShock4Buttons::NO_BUTTONS)
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