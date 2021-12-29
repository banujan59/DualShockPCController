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
	m_previousIterationButtonDown(0),
	m_timeButtonSpentDown(0),
	m_gyroControlledMouseEnabled(false)
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
		const JOY_SHOCK_STATE joyState = JslGetSimpleState(m_nConnectedDeviceID);
		
		// Update mouse position
		m_currentButtonHandler->UpdateMousePosWithJoySticks(joyState.stickLX, joyState.stickLY);

		if(m_gyroControlledMouseEnabled)
		{
			const IMU_STATE  imuState = JslGetIMUState(m_nConnectedDeviceID);
			CustomButtonHandler::UpdateMouseWIthGyro(imuState.gyroX, imuState.gyroY);
		}

		// update scrollwheel
		m_currentButtonHandler->UpdateMouseScrollWithJoySticks(joyState.stickRX, joyState.stickRY);


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

std::vector<std::string> DualShockController::GetButtonConfigurationNames() const
{
	std::vector<std::string> configurationNames;

	std::ranges::for_each(m_availableButtonHandlers, 
      [&configurationNames](const CustomButtonHandler& buttonHandler)
      {
          configurationNames.push_back(buttonHandler.GetButtonLayoutName());
      }
	);

	return configurationNames;
}

void DualShockController::EnableGryoControlledMouse(bool enable)
{
	m_gyroControlledMouseEnabled = enable;
	if(m_gyroControlledMouseEnabled)
	{
		JslStartContinuousCalibration(m_nConnectedDeviceID);
	}

	else
	{
		JslPauseContinuousCalibration(m_nConnectedDeviceID);
		JslResetContinuousCalibration(m_nConnectedDeviceID);
	}
}

bool DualShockController::IsGyroControlledMouseEnabled() const
{
	return m_gyroControlledMouseEnabled;
}

int DualShockController::GetMouseAccelerationFactor() const
{
	return m_currentButtonHandler->GetMouseAccelerationFactor();
}

void DualShockController::SetMouseAccelerationFactor(int newFactor) const
{
	m_currentButtonHandler->SetMouseAccelerationFactor(newFactor);
}

int DualShockController::GetMaxMouseSensitivityFactor()
{
	return MAX_MOUSE_SENSITIVITY;
}

int DualShockController::GetMinMouseSensitivityFactor()
{
	return MIN_MOUSE_SENSITIVITY;
}