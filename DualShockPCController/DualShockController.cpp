#include "DualShockController.h"
#include "JoyShockLibrary.h"
#include "OSHelper.h"

#include <iostream>

namespace 
{
	constexpr int THREAD_FUNCTION_SLEEP_INTERVAL_MILLISECONDS = 10;
	constexpr int MAX_MOUSE_SENSITIVITY = 40;
	constexpr int MIN_MOUSE_SENSITIVITY = 5;

	enum ControllerType
	{
		UnknownController,
		LeftJoyCon,
		RightJoyCon,
		SwitchProController,
		DualShock4,
		DualSense
	};
}

DualShockController::DualShockController() :
	m_nConnectedDeviceID(-1),
	m_bContinueThreadExecution(false),
	m_pThread(nullptr),
	m_mouseAccelerationFactor(20),
	m_previousIterationButtonDown(0),
	m_timeButtonSpentDown(0)
{
	// Load list of available button handlers
	m_availableButtonHandlers.push_back(DefaultConfigButtonHandler());

	// Load the default button handler on startup
	m_currentButtonHandler = &m_availableButtonHandlers[0];
}

DualShockController::~DualShockController()
{
	// stop thread function and wait for it to finish
	if(m_pThread != nullptr)
	{
		m_bContinueThreadExecution = false;
		m_pThread->join();
	}

	JslDisconnectAndDisposeAll();
}

bool DualShockController::ConnectToDevice()
{
	const int nDevices = JslConnectDevices();
	if (nDevices == 0)
	{
		std::cerr << "No devices found!" << std::endl;
		return false;
	}

	const std::unique_ptr<int> pDeviceHandleArray(new int(nDevices));

	if (JslGetConnectedDeviceHandles(pDeviceHandleArray.get(), nDevices))
	{
		for(int i = 0 ; i < nDevices ; i++)
		{
			int type = JslGetControllerType(pDeviceHandleArray.get()[i]);
			if (type == ControllerType::DualShock4)
			{
				m_nConnectedDeviceID = pDeviceHandleArray.get()[i];

				m_bContinueThreadExecution = true;
				m_pThread.reset(new std::thread(&DualShockController::_CaptureEvents, this));
				return true;
			}
		}
	}

	std::cerr << "Could not detect a Dualshock 4 device" << std::endl;
	JslDisconnectAndDisposeAll();
	return false;
}

void DualShockController::_CaptureEvents()
{
	while(m_bContinueThreadExecution)
	{
		JOY_SHOCK_STATE joyState = JslGetSimpleState(m_nConnectedDeviceID);
		
		// Update mouse position
		MousePosition oMousePosition = GetCurrentMousePosition();
		oMousePosition.x = oMousePosition.x + static_cast<long>(static_cast<float>(m_mouseAccelerationFactor) * joyState.stickLX);
		oMousePosition.y = oMousePosition.y - static_cast<long>(static_cast<float>(m_mouseAccelerationFactor) * joyState.stickLY);
		SetNewMousePosition(oMousePosition);

		// update scrollwheel
		TriggerVerticalScroll(joyState.stickRY);
		TriggerHorizontalScroll(joyState.stickRX);


		if(m_previousIterationButtonDown != joyState.buttons)
		{
			m_currentButtonHandler->OnKeyUp(m_previousIterationButtonDown, m_timeButtonSpentDown);
			m_timeButtonSpentDown = 0;
			m_currentButtonHandler->OnKeyDown(joyState.buttons);
		}

		else
		{
			m_timeButtonSpentDown += THREAD_FUNCTION_SLEEP_INTERVAL_MILLISECONDS;
		}

		m_previousIterationButtonDown = joyState.buttons;
		std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_FUNCTION_SLEEP_INTERVAL_MILLISECONDS));
	}
}

int DualShockController::GetMouseAccelerationFactor()
{
	return m_mouseAccelerationFactor;
}

void DualShockController::SetMouseAccelerationFactor(int newFactor)
{
	m_mouseAccelerationFactor = newFactor;
}

int DualShockController::GetMaxMouseSensitivityFactor()
{
	return MAX_MOUSE_SENSITIVITY;
}

int DualShockController::GetMinMouseSensitivityFactor()
{
	return MIN_MOUSE_SENSITIVITY;
}