#pragma once

#include <thread>

class DualShockController
{
private:
	int m_nConnectedDeviceID;
	bool m_bContinueExecution;
	std::unique_ptr<std::thread> m_pThread;

	void _CaptureEvents();

public:
	DualShockController();
	~DualShockController();
	bool ConnectToDevice();
};

