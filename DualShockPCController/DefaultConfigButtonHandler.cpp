#include "DefaultConfigButtonHandler.h"
#include "OSHelper.h"

DefaultConfigButtonHandler::DefaultConfigButtonHandler()
{
	// X to left mouse button
	AddButtonDownMapping(DualShock4Buttons::X, TriggerMouseLeftDown);
	AddButtonUpMapping(DualShock4Buttons::X, TriggerMouseLeftUp);

	// Square as window maximize / restore
	AddShortButtonUpMapping(DualShock4Buttons::SQUARE, ToggleActiveWindowMaximized);

	// Triangle as window minimize / restore
	AddShortButtonUpMapping(DualShock4Buttons::TRIANGLE, ToggleActiveWindowMinimized);

	// Circle as close window
	AddShortButtonUpMapping(DualShock4Buttons::CIRCLE, CloseActiveWindow);

	// L1 short press as navigation back
	AddShortButtonUpMapping(DualShock4Buttons::L1, TriggerNavigationBack);

	// L2 short press as media back
	AddShortButtonUpMapping(DualShock4Buttons::L2, TriggerMediaBack);

	// R1 short press as navigation forward
	AddShortButtonUpMapping(DualShock4Buttons::R1, TriggerNavigationForward);

	// R2 short press as media next
	AddShortButtonUpMapping(DualShock4Buttons::R2, TriggerMediaNext);

	// R2 long press as enter
	AddLongButtonUpMapping(DualShock4Buttons::R2, TriggerEnterButton);

	// D pad arrow as arrow key
	AddShortButtonUpMapping(DualShock4Buttons::DPAD_LEFT, TriggerLeftArrowKey);
	AddShortButtonUpMapping(DualShock4Buttons::DPAD_UP, TriggerUpArrowKey);
	AddShortButtonUpMapping(DualShock4Buttons::DPAD_RIGHT, TriggerRightArrowKey);
	AddShortButtonUpMapping(DualShock4Buttons::DPAD_DOWN, TriggerDownArrowKey);

	// L3 to left mouse button
	AddButtonDownMapping(DualShock4Buttons::L3, TriggerMouseLeftDown);
	AddButtonUpMapping(DualShock4Buttons::L3, TriggerMouseLeftUp);

	// R3 to right mouse button
	AddButtonDownMapping(DualShock4Buttons::R3, TriggerMouseRightDown);
	AddButtonUpMapping(DualShock4Buttons::R3, TriggerMouseRightUp);

	// L1 + R1 to center mouse
	AddLongButtonUpMapping(L1 | R1, CenterMouseToScreen);
}