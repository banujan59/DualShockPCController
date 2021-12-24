#pragma once

class DualShockController
{
private:
	int m_nConnectedDeviceID;

public:
	DualShockController();
	~DualShockController();
	bool ConnectToDevice();
};

