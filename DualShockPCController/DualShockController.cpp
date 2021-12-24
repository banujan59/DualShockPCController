#include "DualShockController.h"
#include "JoyShockLibrary.h"

#include <iostream>

namespace 
{
	constexpr int THREAD_FUNCTION_SLEEP_INTERVAL_MILLISECONDS = 100;

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

DualShockController::DualShockController():
	m_oContinueDestructionSemaphore{0}
{
	m_nConnectedDeviceID = -1;
}

DualShockController::~DualShockController()
{
	// stop thread function and wait for it to finish
	m_bContinueExecution = false;
	m_oContinueDestructionSemaphore.acquire();

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
				
				std::thread(_CaptureEvents);
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
	m_bContinueExecution = true;
	while(m_bContinueExecution)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_FUNCTION_SLEEP_INTERVAL_MILLISECONDS));
	}

	m_oContinueDestructionSemaphore.release();
}