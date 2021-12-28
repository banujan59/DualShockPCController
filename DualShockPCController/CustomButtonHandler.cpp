#include "CustomButtonHandler.h"
#include "OSHelper.h"

CustomButtonHandler::CustomButtonHandler() :
	m_mouseAccelerationFactor(20)
{
	
}

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

void CustomButtonHandler::CenterMouseToScreen()
{
	long width, height;
	GetScreenResolution(width, height);

	MousePosition position;
	position.x = width / 2;
	position.y = height / 2;
	SetNewMousePosition(position);
}

void CustomButtonHandler::UpdateMousePosWithJoySticks(float stickLX, float stickLY) const
{
	MousePosition oMousePosition = GetCurrentMousePosition();
	oMousePosition.x = oMousePosition.x + static_cast<long>(static_cast<float>(m_mouseAccelerationFactor) * stickLX);
	oMousePosition.y = oMousePosition.y - static_cast<long>(static_cast<float>(m_mouseAccelerationFactor) * stickLY);
	SetNewMousePosition(oMousePosition);
}

void CustomButtonHandler::UpdateMouseScrollWithJoySticks(float stickRX, float stickRY) const
{
	TriggerVerticalScroll(stickRY);
	TriggerHorizontalScroll(stickRX);
}

void CustomButtonHandler::UpdateMouseWIthGyro(float gyroX, float gyroY)
{
	MousePosition oMousePosition = GetCurrentMousePosition();

	oMousePosition.x -= static_cast<long>(gyroY);
	oMousePosition.y -= static_cast<long>(gyroX);

	// if we are out of window boundaries, stop
	long windowWidth, windowHeight;
	GetScreenResolution(windowWidth, windowHeight);

	if (oMousePosition.x >= 0 && oMousePosition.x <= windowWidth &&
		oMousePosition.y >= 0 && oMousePosition.y <= windowHeight)
	{
		SetNewMousePosition(oMousePosition);
	}
}

int CustomButtonHandler::GetMouseAccelerationFactor() const
{
	return m_mouseAccelerationFactor;
}

void CustomButtonHandler::SetMouseAccelerationFactor(int newFactor)
{
	m_mouseAccelerationFactor = newFactor;
}