#pragma once

#include <thread>
#include "DefaultConfigButtonHandler.h"

class DualShockController
{
private:
	int m_nConnectedDeviceID;
	bool m_bContinueThreadExecution;
	std::unique_ptr<std::thread> m_pThread;

	int m_mouseAccelerationFactor;
	int m_previousIterationButtonDown;
	int m_timeButtonSpentDown;

	CustomButtonHandler* m_currentButtonHandler;
	std::vector<CustomButtonHandler> m_availableButtonHandlers;

	void _CaptureEvents();

public:
	DualShockController();
	~DualShockController();
	bool ConnectToDevice();

	int GetMouseAccelerationFactor();
	void SetMouseAccelerationFactor(int newFactor);

	static int GetMaxMouseSensitivityFactor();
	static int GetMinMouseSensitivityFactor();
};
