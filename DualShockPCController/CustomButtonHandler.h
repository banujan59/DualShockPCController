#pragma once
#include <unordered_map>
#include <functional>

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

class CustomButtonHandler
{
private:
	static constexpr int LONG_PRESS_DURATION_THRESHOLD_MS = 250;

	std::unordered_map<int, std::function<void()>> m_cButtonDownFunctionMap;
	std::unordered_map<int, std::function<void()>> m_cShortButtonUpFunctionMap;
	std::unordered_map<int, std::function<void()>> m_cLongButtonUpFunctionMap;

	int m_mouseAccelerationFactor;
	std::string m_buttonLayoutName;

public:
	CustomButtonHandler(std::string buttonLayoutName);

	void AddButtonDownMapping(int button, std::function<void()> function);
	void AddShortButtonUpMapping(int button, std::function<void()> function);
	void AddLongButtonUpMapping(int button, std::function<void()> function);
	void AddButtonUpMapping(int button, std::function<void()> function);
	

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


	static void CenterMouseToScreen();
	void UpdateMousePosWithJoySticks(float stickLX, float stickLY) const;
	void UpdateMouseScrollWithJoySticks(float stickRX, float stickRY) const;
	static void UpdateMouseWIthGyro(float gyroX, float gyroY);

	std::string GetButtonLayoutName() const;
	int GetMouseAccelerationFactor() const;
	void SetMouseAccelerationFactor(int newFactor);
};

