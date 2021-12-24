#pragma once

struct MousePosition {
	long x = 0;
	long y = 0;
};

/*
 * These function are platform specific.
 */

MousePosition GetCurrentMousePosition();
void SetNewMousePosition(MousePosition& oNewPosition);