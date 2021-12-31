#include "DualShockController.h"
#include "JoyShockLibrary.h"

#include <iostream>
#include <semaphore>

namespace 
{
	constexpr int THREAD_FUNCTION_SLEEP_INTERVAL_MILLISECONDS = 10;
	constexpr int CUSTOM_BUTTON_SEQUENCE_ACTIVATE_DELAY = 500;

	constexpr int RUMBLE_SLEEP_DURATION_MILLISECONDS = 50;
	std::binary_semaphore rumbleSemaphore{ 0 };

	int timeWithNoButton = 0;

	bool DSButtonSequenceMode = false;

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
	m_bContinueCaptureThreadExecution(false),
	m_pCaptureDSEventThread(nullptr),
	m_pRumbleThread(nullptr),
	m_previousIterationButtonDown(0),
	m_timeButtonSpentDown(0),
	m_gyroControlledMouseEnabled(false)
{
	// Load list of available button handlers
	m_availableButtonHandlers.push_back(DefaultConfigButtonHandler());

	// Load the default button handler on startup
	m_currentButtonHandler = &m_availableButtonHandlers[0];

	// small trick to start the rumble thread. (I know, not ideal...)
	SetRumbleSensitivity( m_currentButtonHandler->GetRumbleSensitivity() ); 
}

DualShockController::~DualShockController()
{
	// stop thread function and wait for it to finish
	if(m_pCaptureDSEventThread != nullptr)
	{
		m_bContinueCaptureThreadExecution = false;
		m_pCaptureDSEventThread->join();
	}

	if(m_pRumbleThread != nullptr)
	{
		m_bContinueRumbleThreadExecution = false;
		rumbleSemaphore.release();
		m_pRumbleThread->join();
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

				m_bContinueCaptureThreadExecution = true;
				m_pCaptureDSEventThread.reset(new std::thread(&DualShockController::_CaptureEvents, this));
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
	while(m_bContinueCaptureThreadExecution)
	{
		const JOY_SHOCK_STATE joyState = JslGetSimpleState(m_nConnectedDeviceID);
		
		// Update mouse position
		m_currentButtonHandler->UpdateMousePosWithJoySticks(joyState.stickLX, joyState.stickLY);

		if(m_gyroControlledMouseEnabled)
		{
			const IMU_STATE  imuState = JslGetIMUState(m_nConnectedDeviceID);
			CustomButtonConfiguration::UpdateMouseWIthGyro(imuState.gyroX, imuState.gyroY);
		}

		// update scrollwheel
		m_currentButtonHandler->UpdateMouseScrollWithJoySticks(joyState.stickRX, joyState.stickRY);


		if(m_previousIterationButtonDown != joyState.buttons)
		{
			// if we are entering a new button sequence with the controller for a custom command, ignore the output to the button handler
			if(!DSButtonSequenceMode)
			{
				m_currentButtonHandler->OnKeyUp(m_previousIterationButtonDown, m_timeButtonSpentDown);

				if (m_bContinueRumbleThreadExecution)
					rumbleSemaphore.release();

				m_timeButtonSpentDown = 0;
				m_currentButtonHandler->OnKeyDown(joyState.buttons);

				if (joyState.buttons == DualShock4Buttons::NO_BUTTONS)
					timeWithNoButton = 0;
			}
		}

		else
		{
			m_timeButtonSpentDown += THREAD_FUNCTION_SLEEP_INTERVAL_MILLISECONDS;
		}

		if(joyState.buttons == DualShock4Buttons::NO_BUTTONS && timeWithNoButton < CUSTOM_BUTTON_SEQUENCE_ACTIVATE_DELAY)
		{
			timeWithNoButton += THREAD_FUNCTION_SLEEP_INTERVAL_MILLISECONDS;

			if(timeWithNoButton >= CUSTOM_BUTTON_SEQUENCE_ACTIVATE_DELAY)
			{
				m_currentButtonHandler->ActivateCustomButtonSequence();
			}
		}

		m_previousIterationButtonDown = joyState.buttons;
		std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_FUNCTION_SLEEP_INTERVAL_MILLISECONDS));
	}
}

void DualShockController::_RunDSRumble() const
{
	while(m_bContinueRumbleThreadExecution)
	{
		rumbleSemaphore.acquire();

		// the semaphore could have been released to end this thread
		if(m_bContinueRumbleThreadExecution)
		{
			int rumbleValue = m_currentButtonHandler->GetRumbleSensitivity();
			SendRumbleToDS(rumbleValue);
		}
	}
}

std::vector<std::string> DualShockController::GetButtonConfigurationNames() const
{
	std::vector<std::string> configurationNames;

	std::ranges::for_each(m_availableButtonHandlers, 
      [&configurationNames](const CustomButtonConfiguration& buttonHandler)
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

int DualShockController::GetCurrentRumbleSensitivity() const
{
	return m_currentButtonHandler->GetRumbleSensitivity();
}

void DualShockController::SetRumbleSensitivity(const unsigned int& newSensitivity)
{
	if(newSensitivity == MIN_RUMBLE_SENSITIVITY && m_pRumbleThread != nullptr)
	{
		m_bContinueRumbleThreadExecution = false;
		rumbleSemaphore.release();
		m_pRumbleThread->join();
		m_pRumbleThread.reset(nullptr);
	}

	else if(m_pRumbleThread == nullptr)
	{
		m_bContinueRumbleThreadExecution = true;
		m_pRumbleThread.reset(new std::thread(&DualShockController::_RunDSRumble, this));
	}

	m_currentButtonHandler->SetRumbleSensitivity(newSensitivity);
}

void DualShockController::SendRumbleToDS(int& rumbleValue) const
{
	int smallRumble, bigRumble;
	CustomButtonConfiguration::GetRumbleValueForDS(smallRumble, bigRumble, rumbleValue);

	JslSetRumble(m_nConnectedDeviceID, smallRumble, bigRumble);
	std::this_thread::sleep_for(std::chrono::milliseconds(RUMBLE_SLEEP_DURATION_MILLISECONDS));
	JslSetRumble(m_nConnectedDeviceID, 0, 0);
}

void DualShockController::GetAllCustomCommands(std::vector<std::string>& commandNames, std::vector<std::string>& buttonList,
                                               std::vector<std::string>& actionType) const
{
	m_currentButtonHandler->GetAllCustomCommands(commandNames, buttonList, actionType);
}

void DualShockController::RemoveCustomCommand(std::string& commandName) const
{
	m_currentButtonHandler->RemoveCustomCommand(commandName);
}

void DualShockController::GetCustomCommandsActions(std::map<CustomButtonSequence::ActionType, std::string>& container)
{
	CustomButtonSequence::GetActionTypeNames(container);
}

void DualShockController::GetDSButtonNames(std::map<int, std::string>& container)
{
	CustomButtonConfiguration::GetDSButtonNames(container);
}

bool DualShockController::AddNewCustomCommand(std::string& commmandName, std::vector<int>& buttonSequence,
                                              CustomButtonSequence::ActionType& actionType, std::vector<
	                                              std::string>& actionTypeParameters) const
{
	return m_currentButtonHandler->AddNewCustomCommand(commmandName, buttonSequence, actionType, actionTypeParameters);
}

void DualShockController::SetDSButtonSequenceMode(bool state)
{
	DSButtonSequenceMode = state;
}

int DualShockController::GetLatestButtonDown() const
{
	return m_previousIterationButtonDown;
}
