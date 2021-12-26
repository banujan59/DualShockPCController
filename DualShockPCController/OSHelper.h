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