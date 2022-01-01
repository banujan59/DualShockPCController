#pragma once

#include <thread>
#include "DefaultConfigButtonHandler.h"

class DualShockController
{
private:
	int m_nConnectedDeviceID;
	bool m_bContinueCaptureThreadExecution;
	bool m_bContinueRumbleThreadExecution;
	std::unique_ptr<std::thread> m_pCaptureDSEventThread;
	std::unique_ptr<std::thread> m_pRumbleThread;
	
	int m_previousIterationButtonDown;
	int m_timeButtonSpentDown;

	bool m_gyroControlledMouseEnabled;

	CustomButtonConfiguration* m_currentButtonHandler;
	std::vector<CustomButtonConfiguration> m_availableButtonHandlers;

	void _CaptureEvents();
	void _RunDSRumble() const;

public:
	static constexpr int MAX_MOUSE_SENSITIVITY = 40;
	static constexpr int MIN_MOUSE_SENSITIVITY = 5;

	static constexpr int MAX_RUMBLE_SENSITIVITY = 4;
	static constexpr int MIN_RUMBLE_SENSITIVITY = 0; // the minimum must always be 0. This value is used to kill the rumble thread

	DualShockController();
	~DualShockController();
	bool ConnectToDevice();

	std::vector<std::string> GetButtonConfigurationNames() const;

	void EnableGryoControlledMouse(bool enable);
	bool IsGyroControlledMouseEnabled() const;

	int GetMouseAccelerationFactor() const;
	void SetMouseAccelerationFactor(int newFactor) const;

	int GetCurrentRumbleSensitivity() const;
	void SetRumbleSensitivity(const unsigned int& newSensitivity);
	void SendRumbleToDS(int& rumbleValue) const;

	void GetAllCustomCommands(std::vector<std::string>& commandNames, std::vector<std::string>& buttonList, std::vector<std::string>& actionType) const;
	void RemoveCustomCommand(std::string& commandName) const;
	static void GetCustomCommandsActions(std::map<CustomButtonSequence::ActionType, std::string>& container);
	static void GetDSButtonNames(std::map<int, std::string>& container);
	bool AddNewCustomCommand(std::string& commmandName, std::vector<int>& buttonSequence,
	                         CustomButtonSequence::ActionType& actionType, std::vector<
		                         std::string>& actionTypeParameters) const;

	void SetDSButtonSequenceMode(bool state);
	int GetLatestButtonDown() const;
};
