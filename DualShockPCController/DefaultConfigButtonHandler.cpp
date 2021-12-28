#include "DefaultConfigButtonHandler.h"
#include "OSHelper.h"

DefaultConfigButtonHandler::DefaultConfigButtonHandler()
{
	// X to left mouse button
	AddButtonDownMapping(DualShock4Buttons::X, OSHelper::TriggerMouseLeftDown);
	AddButtonUpMapping(DualShock4Buttons::X, OSHelper::TriggerMouseLeftUp);

	// Square as window maximize / restore
	AddShortButtonUpMapping(DualShock4Buttons::SQUARE, OSHelper::ToggleActiveWindowMaximized);

	// Triangle as window minimize / restore
	AddShortButtonUpMapping(DualShock4Buttons::TRIANGLE, OSHelper::ToggleActiveWindowMinimized);

	// Circle as close window
	AddShortButtonUpMapping(DualShock4Buttons::CIRCLE, OSHelper::CloseActiveWindow);

	// L1 short press as navigation back
	AddShortButtonUpMapping(DualShock4Buttons::L1, OSHelper::TriggerNavigationBack);

	// L2 short press as media back
	AddShortButtonUpMapping(DualShock4Buttons::L2, OSHelper::TriggerMediaBack);

	// R1 short press as navigation forward
	AddShortButtonUpMapping(DualShock4Buttons::R1, OSHelper::TriggerNavigationForward);

	// R2 short press as media next
	AddShortButtonUpMapping(DualShock4Buttons::R2, OSHelper::TriggerMediaNext);

	// R2 long press as enter
	AddLongButtonUpMapping(DualShock4Buttons::R2, OSHelper::TriggerEnterButton);

	// D pad arrow as arrow key
	AddShortButtonUpMapping(DualShock4Buttons::DPAD_LEFT, OSHelper::TriggerLeftArrowKey);
	AddShortButtonUpMapping(DualShock4Buttons::DPAD_UP, OSHelper::TriggerUpArrowKey);
	AddShortButtonUpMapping(DualShock4Buttons::DPAD_RIGHT, OSHelper::TriggerRightArrowKey);
	AddShortButtonUpMapping(DualShock4Buttons::DPAD_DOWN, OSHelper::TriggerDownArrowKey);

	// L3 to left mouse button
	AddButtonDownMapping(DualShock4Buttons::L3, OSHelper::TriggerMouseLeftDown);
	AddButtonUpMapping(DualShock4Buttons::L3, OSHelper::TriggerMouseLeftUp);

	// R3 to right mouse button
	AddButtonDownMapping(DualShock4Buttons::R3, OSHelper::TriggerMouseRightDown);
	AddButtonUpMapping(DualShock4Buttons::R3, OSHelper::TriggerMouseRightUp);

	// center to center mouse
	AddLongButtonUpMapping(CENTER_TOUCH_BAR, CenterMouseToScreen);
}