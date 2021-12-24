#pragma once

#include <thread>

class DualShockController
{
private:
	int m_nConnectedDeviceID;
	bool m_bContinueThreadExecution;
	std::unique_ptr<std::thread> m_pThread;

	float m_mouseAccelerationFactor = 20.0f;

	void _CaptureEvents();

public:
	DualShockController();
	~DualShockController();
	bool ConnectToDevice();
};

