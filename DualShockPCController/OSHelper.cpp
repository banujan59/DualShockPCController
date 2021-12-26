#include "OSHelper.h"
#include <Windows.h>

/*
 * Implementation for Windows.
 */

MousePosition GetCurrentMousePosition()
{
	MousePosition oMousePosition;

	POINT oPoint;
	if (GetCursorPos(&oPoint)) {
		oMousePosition.x = oPoint.x;
		oMousePosition.y = oPoint.y;
	}

	return oMousePosition;
}

void SetNewMousePosition(MousePosition& oNewPosition)
{
	SetCursorPos(oNewPosition.x, oNewPosition.y);
}

void TriggerMouseLeftDown()
{
	INPUT Inputs[1];
	Inputs[0].type = INPUT_MOUSE;
	Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, Inputs, sizeof(INPUT));
}

void TriggerMouseLeftUp()
{
	INPUT Inputs[1];
	Inputs[0].type = INPUT_MOUSE;
	Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, Inputs, sizeof(INPUT));
}

void TriggerMouseRightDown()
{
	INPUT Inputs[1];
	Inputs[0].type = INPUT_MOUSE;
	Inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	SendInput(1, Inputs, sizeof(INPUT));
}

void TriggerMouseRightUp()
{
	INPUT Inputs[1];
	Inputs[0].type = INPUT_MOUSE;
	Inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	SendInput(1, Inputs, sizeof(INPUT));
}

void TriggerVerticalScroll(float dScrollValue)
{
	mouse_event(MOUSEEVENTF_WHEEL, 0, 0, static_cast<int>(25 * dScrollValue), 0);
}

void TriggerHorizontalScroll(float dScrollValue)
{
	mouse_event(MOUSEEVENTF_HWHEEL, 0, 0, static_cast<int>(25 * dScrollValue), 0);
}

void ToggleActiveWindowMaximized()
{
	HWND windowHandler = GetForegroundWindow();
	WINDOWPLACEMENT windowPlacement;
	if (GetWindowPlacement(windowHandler, &windowPlacement))
	{
		if (windowPlacement.showCmd == SW_SHOWNORMAL)
		{
			ShowWindow(windowHandler, SW_MAXIMIZE);
		}

		else if (windowPlacement.showCmd == SW_SHOWMAXIMIZED || windowPlacement.showCmd == SW_SHOWMINIMIZED)
		{
			ShowWindow(windowHandler, SW_RESTORE);
		}
	}
}

void ToggleActiveWindowMinimized()
{
	HWND windowHandler = GetForegroundWindow();
	WINDOWPLACEMENT windowPlacement;
	if (GetWindowPlacement(windowHandler, &windowPlacement))
	{
		if (windowPlacement.showCmd == SW_SHOWMINIMIZED)
		{
			ShowWindow(windowHandler, SW_RESTORE);
		}

		else if (windowPlacement.showCmd == SW_SHOWNORMAL || windowPlacement.showCmd == SW_SHOWMAXIMIZED)
		{
			ShowWindow(windowHandler, SW_MINIMIZE);
		}
	}
}

void CloseActiveWindow()
{
	HWND windowHandler = GetForegroundWindow();
	PostMessage(windowHandler, WM_CLOSE, 0, 0);
}

void TriggerLeftArrowKey()
{
	INPUT Inputs[1];
	Inputs[0].type = INPUT_KEYBOARD;
	Inputs[0].ki.wVk = VK_LEFT;
	Inputs[0].ki.dwFlags = WM_KEYUP;

	SendInput(1, Inputs, sizeof(INPUT));
}

void TriggerRightArrowKey()
{
	INPUT Inputs[1];
	Inputs[0].type = INPUT_KEYBOARD;
	Inputs[0].ki.wVk = VK_RIGHT;
	Inputs[0].ki.dwFlags = WM_KEYUP;
	SendInput(1, Inputs, sizeof(INPUT));
}

void TriggerUpArrowKey()
{
	INPUT Inputs[1];
	Inputs[0].type = INPUT_KEYBOARD;
	Inputs[0].ki.wVk = VK_UP;
	Inputs[0].ki.dwFlags = WM_KEYUP;
	SendInput(1, Inputs, sizeof(INPUT));
}

void TriggerDownArrowKey()
{
	INPUT Inputs[1];
	Inputs[0].type = INPUT_KEYBOARD;
	Inputs[0].ki.wVk = VK_DOWN;
	Inputs[0].ki.dwFlags = WM_KEYUP;
	SendInput(1, Inputs, sizeof(INPUT));
}

void TriggerNavigationBack()
{
	INPUT Inputs[3];
	Inputs[0].type = INPUT_KEYBOARD;
	Inputs[0].ki.wVk = VK_MENU;
	Inputs[0].ki.dwFlags = WM_KEYDOWN;

	Inputs[1].type = INPUT_KEYBOARD;
	Inputs[1].ki.wVk = VK_LEFT;
	Inputs[1].ki.dwFlags = WM_KEYDOWN;

	Inputs[2].type = INPUT_KEYBOARD;
	Inputs[2].ki.wVk = VK_MENU;
	Inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(3, Inputs, sizeof(INPUT));
}

void TriggerNavigationForward()
{
	INPUT Inputs[3];
	Inputs[0].type = INPUT_KEYBOARD;
	Inputs[0].ki.wVk = VK_MENU;
	Inputs[0].ki.dwFlags = WM_KEYDOWN;

	Inputs[1].type = INPUT_KEYBOARD;
	Inputs[1].ki.wVk = VK_RIGHT;
	Inputs[1].ki.dwFlags = WM_KEYDOWN;

	Inputs[2].type = INPUT_KEYBOARD;
	Inputs[2].ki.wVk = VK_MENU;
	Inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(3, Inputs, sizeof(INPUT));
}