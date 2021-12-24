#pragma once

#include <thread>
#include <semaphore>

class DualShockController
{
private:
	int m_nConnectedDeviceID;
	bool m_bContinueExecution;
	std::binary_semaphore oContinueDestructionSemaphore;

	void _CaptureEvents();

public:
	DualShockController();
	~DualShockController();
	bool ConnectToDevice();
};

