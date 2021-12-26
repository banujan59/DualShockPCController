#pragma once

#include <thread>

class DualShockController
{
private:
	int m_nConnectedDeviceID;
	bool m_bContinueThreadExecution;
	std::unique_ptr<std::thread> m_pThread;

	int m_mouseAccelerationFactor;
	int m_previousIterationButtonDown;

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

