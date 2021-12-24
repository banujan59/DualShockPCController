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