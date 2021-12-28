#pragma once

#include <thread>
#include "DefaultConfigButtonHandler.h"

class DualShockController
{
private:
	int m_nConnectedDeviceID;
	bool m_bContinueThreadExecution;
	std::unique_ptr<std::thread> m_pThread;
	
	int m_previousIterationButtonDown;
	int m_timeButtonSpentDown;

	bool m_gyroControlledMouseEnabled;

	CustomButtonHandler* m_currentButtonHandler;
	std::vector<CustomButtonHandler> m_availableButtonHandlers;

	void _CaptureEvents();

public:
	DualShockController();
	~DualShockController();
	bool ConnectToDevice();

	void EnableGryoControlledMouse(bool enable);
	bool IsGyroControlledMouseEnabled() const;

	int GetMouseAccelerationFactor() const;
	void SetMouseAccelerationFactor(int newFactor) const;

	static int GetMaxMouseSensitivityFactor();
	static int GetMinMouseSensitivityFactor();
};
