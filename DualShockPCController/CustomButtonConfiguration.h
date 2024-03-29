#pragma once
#include <unordered_map>
#include <boost/serialization/unordered_map.hpp>

#include "CustomButtonSequence.h"

enum DualShock4Buttons
{
	NO_BUTTONS = 0x0,
	DPAD_UP = 0x00001,
	DPAD_DOWN = 0x00002,
	DPAD_LEFT = 0x00004,
	DPAD_RIGHT = 0x00008,
	OPTIONS = 0x00010,
	SHARE = 0x00020,
	L3 = 0x00040,
	R3 = 0x00080,
	L1 = 0x00100,
	R1 = 0x00200,
	L2 = 0x00400,
	R2 = 0x00800,
	X = 0x01000,
	CIRCLE = 0x02000,
	SQUARE = 0x04000,
	TRIANGLE = 0x08000,
	PS_BUTTON = 0x10000,
	CENTER_TOUCH_BAR = 0x20000
};

enum class LightBarMode
{
	SINGLE_COLOR,
	RANDOM_COLOR
};

class CustomButtonConfiguration
{
private:
	/// <summary>
	/// This constructor should not be used by the application. It is used for the serialization only.
	/// </summary>
	CustomButtonConfiguration()
	{
		// to remove the warnings... these will be overwritten during deserialization
		m_rumbleSensitivity = 0;
		m_mouseAccelerationFactor = 0;
		m_lightBarFadeEnabled = false;
		m_lightBarColor = 0;
		m_lightBarMode = LightBarMode::SINGLE_COLOR;
		m_customButtonSequenceModeEnabled = false;
		m_gyroControlledMouseEnabled = false;
	}

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& m_cButtonDownFunctionMap;
		ar& m_cShortButtonUpFunctionMap;
		ar& m_cLongButtonUpFunctionMap;

		ar& m_mouseAccelerationFactor;
		ar& m_gyroControlledMouseEnabled;
		ar& m_rumbleSensitivity;
		ar& m_buttonConfigurationName;

		ar& m_customButtonSequence;

		ar& m_lightBarColor;
		ar& m_lightBarFadeEnabled;
		ar& m_lightBarMode;
	}

	static constexpr int LONG_PRESS_DURATION_THRESHOLD_MS = 250;

	std::unordered_map<int, OSHelper::FunctionEnum> m_cButtonDownFunctionMap;
	std::unordered_map<int, OSHelper::FunctionEnum> m_cShortButtonUpFunctionMap;
	std::unordered_map<int, OSHelper::FunctionEnum> m_cLongButtonUpFunctionMap;

	int m_mouseAccelerationFactor;
	bool m_gyroControlledMouseEnabled;
	unsigned int m_rumbleSensitivity;
	std::string m_buttonConfigurationName;

	CustomButtonSequence m_customButtonSequence;
	bool m_customButtonSequenceModeEnabled;

	int m_lightBarColor;
	bool m_lightBarFadeEnabled;
	LightBarMode m_lightBarMode;

public:
	CustomButtonConfiguration(std::string buttonLayoutName);

	void AddButtonDownMapping(int button, OSHelper::FunctionEnum function);
	void AddShortButtonUpMapping(int button, OSHelper::FunctionEnum function);
	void AddLongButtonUpMapping(int button, OSHelper::FunctionEnum function);
	void AddButtonUpMapping(int button, OSHelper::FunctionEnum function);
	

	/// <summary>
	/// Handler for a button down event
	/// </summary>
	/// <param name="buttons">The button that triggered the event</param>
	void OnKeyDown(int buttons);

	/// <summary>
	/// Handler for a button up event
	/// </summary>
	/// <param name="buttons">The button that triggered the event</param>
	/// <param name="durationMS">The time the button spent down before calling this event handler. The value is expected to be in milliseconds.
	/// This will be used to call the correct short press or long press function handler</param>
	void OnKeyUp(int buttons, int durationMS);

	void ActivateCustomButtonSequence();
	
	void UpdateMousePosWithJoySticks(float stickLX, float stickLY) const;
	void UpdateMouseScrollWithJoySticks(float stickRX, float stickRY) const;
	static void UpdateMouseWIthGyro(float gyroX, float gyroY);

	std::string GetButtonLayoutName() const;

	int GetMouseAccelerationFactor() const;
	void SetMouseAccelerationFactor(int newFactor);

	void SetGryoControlledMouseEnabled(bool enable);
	bool IsGyroControlledMouseEnabled() const;

	int GetRumbleSensitivity() const;
	void SetRumbleSensitivity(const unsigned int& newSensitivity);
	static void GetRumbleValueForDS(int& smallRuble, int& bigRumble, int& targetRumbleValue);

	int GetLightBarColor();
	void GetRGBLightBarColor(uint8_t& red, uint8_t& green, uint8_t& blue);
	LightBarMode GetLightBarMode();
	bool GetLightBarFadeEnabled();
	void SetRGBLightBarColor(uint8_t& red, uint8_t& green, uint8_t& blue);
	void SetLightBarMode(LightBarMode& lightBarMode, bool& fadeEnabled);

	void GetAllCustomCommands(std::vector<std::string>& commandNames, std::vector<std::string>& buttonList, std::vector<std::string>& actionType);
	bool AddNewCustomCommand(std::string& commmandName, std::vector<int>& buttonSequence,
	                         CustomButtonSequence::ActionType& actionType,
	                         std::vector<std::string>& actionTypeParameters);
	bool RemoveCustomCommand(std::string& commandName);
	static void GetDSButtonNames(std::map<int, std::string>& container);
};

