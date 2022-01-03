#pragma once
#include <vector>
#include <string>

namespace OSHelper
{
	struct MousePosition {
		long x = 0;
		long y = 0;
	};

	/*
	 * These function are platform specific.
	 */

	MousePosition GetCurrentMousePosition();
	void SetNewMousePosition(MousePosition& oNewPosition);

	void GetScreenResolution(long& windowWidth, long& windowHeight);
	void CenterMouseToScreen();

	void ExecuteApplication(const char* filename);

	void TriggerMouseLeftDown();
	void TriggerMouseLeftUp();
	void TriggerMouseRightDown();
	void TriggerMouseRightUp();

	void TriggerVerticalScroll(float dScrollValue);
	void TriggerHorizontalScroll(float dScrollValue);

	void ToggleActiveWindowMaximized();
	void ToggleActiveWindowMinimized();
	void CloseActiveWindow();

	void TriggerLeftArrowKey();
	void TriggerRightArrowKey();
	void TriggerUpArrowKey();
	void TriggerDownArrowKey();

	void TriggerNavigationBack();
	void TriggerNavigationForward();

	void TriggerMediaBack();
	void TriggerMediaNext();

	void TriggerEnterButton();


	/// <summary>
	/// To help with the serialization process, the controller will keep an enum to the function to execute.
	///	This will also help with portability to Linux.
	/// </summary>
	enum FunctionEnum
	{
		CENTER_MOUSE,
		EXECUTE_APPLICATIONS,
		MOUSE_LEFT_DOWN,
		MOUSE_LEFT_UP,
		MOUSE_RIGHT_DOWN,
		MOUSE_RIGHT_UP,
		TOGGLE_WINDOW_MAXIMIZE,
		TOGGLE_WINDOW_MINIMIZE,
		CLOSE_WINDOW,
		TRIGGER_LEFT_ARROW,
		TRIGGER_UP_ARROW,
		TRIGGER_RIGHT_ARROW,
		TRIGGER_DOWN_ARROW,
		NAVIGATION_BACK,
		NAVIGATION_NEXT,
		MEDIA_BACK,
		MEDIA_NEXT,
		TRIGGER_ENTER
	};

	void ExecuteOSHelperFunction(FunctionEnum functionEnum, std::vector<std::string>* args = nullptr);
}