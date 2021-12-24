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