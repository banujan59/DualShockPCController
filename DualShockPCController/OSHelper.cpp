#include "OSHelper.h"

#include <filesystem>
#include <Windows.h>

/*
 * Implementation for Windows.
 */

namespace
{
	bool mouseLeftDown = false;
	bool mouseRightDown = false;
}

namespace OSHelper
{
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

	void GetScreenResolution(long& windowWidth, long& windowHeight)
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		windowWidth = desktop.right;
		windowHeight = desktop.bottom;
	}

	void CenterMouseToScreen()
	{
		long width, height;
		GetScreenResolution(width, height);

		MousePosition position;
		position.x = width / 2;
		position.y = height / 2;
		SetNewMousePosition(position);
	}

	void ExecuteApplication(const char* filename)
	{
		ShellExecuteA(NULL, "open", filename, "", NULL, SW_NORMAL);
	}

	void TriggerMouseLeftDown()
	{
		if (!mouseLeftDown)
		{
			INPUT Inputs[1];
			Inputs[0].type = INPUT_MOUSE;
			Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			SendInput(1, Inputs, sizeof(INPUT));

			mouseLeftDown = true;
		}
	}

	void TriggerMouseLeftUp()
	{
		if (mouseLeftDown)
		{
			INPUT Inputs[1];
			Inputs[0].type = INPUT_MOUSE;
			Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;
			SendInput(1, Inputs, sizeof(INPUT));

			mouseLeftDown = false;
		}
	}

	void TriggerMouseRightDown()
	{
		if (!mouseRightDown)
		{
			INPUT Inputs[1];
			Inputs[0].type = INPUT_MOUSE;
			Inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
			SendInput(1, Inputs, sizeof(INPUT));

			mouseRightDown = true;
		}
	}

	void TriggerMouseRightUp()
	{
		if (mouseRightDown)
		{
			INPUT Inputs[1];
			Inputs[0].type = INPUT_MOUSE;
			Inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
			SendInput(1, Inputs, sizeof(INPUT));

			mouseRightDown = false;
		}
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
		INPUT Inputs[1];
		Inputs[0].type = INPUT_KEYBOARD;
		Inputs[0].ki.wVk = VK_BROWSER_BACK;
		Inputs[0].ki.dwFlags = WM_KEYUP;
		SendInput(1, Inputs, sizeof(INPUT));
	}

	void TriggerNavigationForward()
	{
		INPUT Inputs[1];
		Inputs[0].type = INPUT_KEYBOARD;
		Inputs[0].ki.wVk = VK_BROWSER_FORWARD;
		Inputs[0].ki.dwFlags = WM_KEYUP;
		SendInput(1, Inputs, sizeof(INPUT));
	}

	void TriggerMediaBack()
	{
		INPUT Inputs[1];
		Inputs[0].type = INPUT_KEYBOARD;
		Inputs[0].ki.wVk = VK_MEDIA_PREV_TRACK;
		Inputs[0].ki.dwFlags = WM_KEYUP;
		SendInput(1, Inputs, sizeof(INPUT));
	}

	void TriggerMediaNext()
	{
		INPUT Inputs[1];
		Inputs[0].type = INPUT_KEYBOARD;
		Inputs[0].ki.wVk = VK_MEDIA_NEXT_TRACK;
		Inputs[0].ki.dwFlags = WM_KEYUP;
		SendInput(1, Inputs, sizeof(INPUT));
	}

	void TriggerEnterButton()
	{
		INPUT Inputs[1];
		Inputs[0].type = INPUT_KEYBOARD;
		Inputs[0].ki.wVk = VK_RETURN;
		Inputs[0].ki.dwFlags = WM_KEYUP;
		SendInput(1, Inputs, sizeof(INPUT));
	}


	void ExecuteOSHelperFunction(FunctionEnum functionEnum, std::vector<std::string>* args)
	{
		switch (functionEnum)
		{
		case CENTER_MOUSE:
			CenterMouseToScreen();
			break;
		case EXECUTE_APPLICATIONS:
			if(args != nullptr)
			{
				for (std::string programToExecute : *args)
				{
					if (std::filesystem::exists(programToExecute))
						OSHelper::ExecuteApplication(programToExecute.c_str());
				}
			}
			break;
		case MOUSE_LEFT_DOWN:
			TriggerMouseLeftDown();
			break;
		case MOUSE_LEFT_UP:
			TriggerMouseLeftUp();
			break;
		case MOUSE_RIGHT_DOWN:
			TriggerMouseRightDown();
			break;
		case MOUSE_RIGHT_UP:
			TriggerMouseRightUp();
			break;
		case TOGGLE_WINDOW_MAXIMIZE:
			ToggleActiveWindowMaximized();
			break;
		case TOGGLE_WINDOW_MINIMIZE:
			ToggleActiveWindowMinimized();
			break;
		case CLOSE_WINDOW:
			CloseActiveWindow();
			break;
		case TRIGGER_LEFT_ARROW:
			TriggerLeftArrowKey();
			break;
		case TRIGGER_UP_ARROW:
			TriggerUpArrowKey();
			break;
		case TRIGGER_RIGHT_ARROW:
			TriggerRightArrowKey();
			break;
		case TRIGGER_DOWN_ARROW:
			TriggerDownArrowKey();
			break;
		case NAVIGATION_BACK:
			TriggerNavigationBack();
			break;
		case NAVIGATION_NEXT:
			TriggerNavigationForward();
			break;
		case MEDIA_BACK:
			TriggerMediaBack();
			break;
		case MEDIA_NEXT:
			TriggerMediaNext();
			break;
		case TRIGGER_ENTER:
			TriggerEnterButton();
			break;
		}
	}
}