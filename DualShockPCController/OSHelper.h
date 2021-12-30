#pragma once
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

	void ExecuteApplication(std::string filename);

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
}