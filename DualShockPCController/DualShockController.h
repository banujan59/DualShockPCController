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

	CustomButtonConfiguration* m_currentButtonHandler;
	std::vector<CustomButtonConfiguration> m_availableButtonHandlers;

	void _CaptureEvents();

public:
	DualShockController();
	~DualShockController();
	bool ConnectToDevice();

	std::vector<std::string> GetButtonConfigurationNames() const;

	void EnableGryoControlledMouse(bool enable);
	bool IsGyroControlledMouseEnabled() const;

	int GetMouseAccelerationFactor() const;
	void SetMouseAccelerationFactor(int newFactor) const;

	static int GetMaxMouseSensitivityFactor();
	static int GetMinMouseSensitivityFactor();

	void GetAllCustomCommands(std::vector<std::string>& commandNames, std::vector<std::string>& buttonList, std::vector<std::string>& actionType) const;
	void RemoveCustomCommand(std::string& commandName) const;
	static void GetCustomCommandsActions(std::map<CustomButtonSequence::ActionType, std::string>& container);
	static void GetDSButtonNames(std::map<int, std::string>& container);
	void AddNewCustomCommand(std::string& commmandName, std::vector<int>& buttonSequence, CustomButtonSequence::ActionType& actionType, std::vector<
	                         std::string>& actionTypeParameters) const;

	void SetDSButtonSequenceMode(bool state);
	int GetLatestButtonDown() const;
};
