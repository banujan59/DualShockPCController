#pragma once

#include <thread>
#include "DefaultConfigButtonHandler.h"

class DualShockController
{
private:
	int m_nConnectedDeviceID;
	bool m_bContinueCaptureThreadExecution;
	bool m_bContinueRumbleAndRandomColorThreadExecution;
	std::unique_ptr<std::thread> m_pCaptureDSEventThread;
	std::unique_ptr<std::thread> m_pRumbleAndRandomColorThread;
	
	int m_previousIterationButtonDown;
	int m_timeButtonSpentDown;

	bool m_gyroControlledMouseEnabled;

	CustomButtonConfiguration* m_currentButtonHandler;
	std::vector<CustomButtonConfiguration> m_availableButtonHandlers;

	/// <summary>
	/// Loads the user's previous settings
	/// </summary>
	/// <returns>True if the user's settings were loaded from the file, false if an error occurred</returns>
	bool LoadData();
	void SaveData();

	void _CaptureEvents();
	void _CaptureRumbleAndRandomLightColor() const;

	void SetLightBarColor() const;
	void UpdateRumbleAndRandomLightColorThread();

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
	void SetMouseAccelerationFactor(int newFactor);

	int GetCurrentRumbleSensitivity() const;
	void SetRumbleSensitivity(const unsigned int& newSensitivity);
	void SendRumbleToDS(int& rumbleValue) const;

	void GetRGBLightBarColor(uint8_t& red, uint8_t& green, uint8_t& blue) const;
	LightBarMode GetLightBarMode() const;
	bool GetLightBarFadeEnabled() const;
	void SetRGBLightBarColor(uint8_t& red, uint8_t& green, uint8_t& blue);
	void SetLightBarMode(LightBarMode& lightBarMode, bool& fadeEnabled);

	void GetAllCustomCommands(std::vector<std::string>& commandNames, std::vector<std::string>& buttonList, std::vector<std::string>& actionType) const;
	void RemoveCustomCommand(std::string& commandName);
	static void GetCustomCommandsActions(std::map<CustomButtonSequence::ActionType, std::string>& container);
	static void GetDSButtonNames(std::map<int, std::string>& container);
	bool AddNewCustomCommand(std::string& commmandName, std::vector<int>& buttonSequence,
	                         CustomButtonSequence::ActionType& actionType, std::vector<
		                         std::string>& actionTypeParameters);

	void SetDSButtonSequenceMode(bool state);
	int GetLatestButtonDown() const;
};
