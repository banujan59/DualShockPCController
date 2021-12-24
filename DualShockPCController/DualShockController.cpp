#include "DualShockController.h"
#include "JoyShockLibrary.h"
#include "OSHelper.h"

#include <iostream>

namespace 
{
	constexpr int THREAD_FUNCTION_SLEEP_INTERVAL_MICROSECONDS = 10000;

	enum ControllerType
	{
		UnknownController,
		LeftJoyCon,
		RightJoyCon,
		SwitchProController,
		DualShock4,
		DualSense
	};

	enum DualShock4Buttons
	{
		NO_BUTTONS = 0x0,
		UP_DPAD = 0x00001,
		DOWN_DPAD = 0x00002,
		LEFT_DPAD = 0x00004,
		RIGHT_DPAD = 0x00008,
		OPTIONS = 0x00010,
		SHARE = 0x00020,
		L3 = 0x00040,
		R3 = 0x00080,
		L1 = 0x00100,
		R1 = 0x00200,
		L2 = 0x00400,
		R2 = 0x00800,
		X = 0x01000,
		CRICLE = 0x02000,
		SQUARE = 0x04000,
		TRIANGLE = 0x08000,
		PS_BUTTON = 0x10000,
		CENTER_TOUCH_BAR = 0x20000
	};
}

DualShockController::DualShockController() : m_pThread(nullptr), m_bContinueThreadExecution(false)
{
	m_nConnectedDeviceID = -1;
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

		std::cerr << "Could not detect a Dualshock 4 device" << std::endl;
		JslDisconnectAndDisposeAll();
		return false;
	}
}

void DualShockController::_CaptureEvents()
{
	while(m_bContinueThreadExecution)
	{
		JOY_SHOCK_STATE joyState = JslGetSimpleState(m_nConnectedDeviceID);

		// Update mouse position
		MousePosition oMousePosition = GetCurrentMousePosition();
		oMousePosition.x = oMousePosition.x + static_cast<long>(m_mouseAccelerationFactor * joyState.stickLX);
		oMousePosition.y = oMousePosition.y - static_cast<long>(m_mouseAccelerationFactor * joyState.stickLY); // y axis is inverted
		SetNewMousePosition(oMousePosition);

		std::this_thread::sleep_for(std::chrono::microseconds(THREAD_FUNCTION_SLEEP_INTERVAL_MICROSECONDS));
	}
}