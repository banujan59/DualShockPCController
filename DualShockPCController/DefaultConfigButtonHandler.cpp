#include "DefaultConfigButtonHandler.h"
#include "OSHelper.h"

DefaultConfigButtonHandler::DefaultConfigButtonHandler() :
	CustomButtonConfiguration("Default Controller")
{
	SetRumbleSensitivity(0);

	// X to left mouse button
	AddButtonDownMapping(DualShock4Buttons::X, OSHelper::FunctionEnum::MOUSE_LEFT_DOWN);
	AddButtonUpMapping(DualShock4Buttons::X, OSHelper::FunctionEnum::MOUSE_LEFT_UP);

	// Square as window maximize / restore
	AddShortButtonUpMapping(DualShock4Buttons::SQUARE, OSHelper::FunctionEnum::TOGGLE_WINDOW_MAXIMIZE);

	// Triangle as window minimize / restore
	AddShortButtonUpMapping(DualShock4Buttons::TRIANGLE, OSHelper::FunctionEnum::TOGGLE_WINDOW_MINIMIZE);

	// Circle as close window
	AddShortButtonUpMapping(DualShock4Buttons::CIRCLE, OSHelper::FunctionEnum::CLOSE_WINDOW);

	// L1 short press as navigation back
	AddShortButtonUpMapping(DualShock4Buttons::L1, OSHelper::FunctionEnum::NAVIGATION_BACK);

	// L2 + left d_pad short press as media back
	AddShortButtonUpMapping(L2 | DPAD_LEFT, OSHelper::FunctionEnum::MEDIA_BACK);

	// L2 + Right dpad short press as media next
	AddShortButtonUpMapping(L2 | DPAD_RIGHT, OSHelper::FunctionEnum::MEDIA_NEXT);

	// L2 + Up dpad short press as media volume up
	AddShortButtonUpMapping(L2 | DPAD_UP, OSHelper::FunctionEnum::MEDIA_VOLUME_UP);
	//
	// L2 + Down dpad short press as media volume down
	AddShortButtonUpMapping(L2 | DPAD_DOWN, OSHelper::FunctionEnum::MEDIA_VOLUME_DOWN);

	// L2 + Down dpad long press as media volume mute
	AddLongButtonUpMapping(L2 | DPAD_DOWN, OSHelper::FunctionEnum::MEDIA_VOLUME_MUTE);

	// L2 + X short press as media volume play/pause
	AddShortButtonUpMapping(L2 | X, OSHelper::FunctionEnum::MEDIA_PAUSE_PLAY_TOGGLE);

	// R1 short press as navigation forward
	AddShortButtonUpMapping(DualShock4Buttons::R1, OSHelper::FunctionEnum::NAVIGATION_NEXT);

	// R2 short press as enter
	AddShortButtonUpMapping(DualShock4Buttons::R2, OSHelper::FunctionEnum::TRIGGER_ENTER);

	// D pad arrow as arrow key
	AddShortButtonUpMapping(DualShock4Buttons::DPAD_LEFT, OSHelper::FunctionEnum::TRIGGER_LEFT_ARROW);
	AddShortButtonUpMapping(DualShock4Buttons::DPAD_UP, OSHelper::FunctionEnum::TRIGGER_UP_ARROW);
	AddShortButtonUpMapping(DualShock4Buttons::DPAD_RIGHT, OSHelper::FunctionEnum::TRIGGER_RIGHT_ARROW);
	AddShortButtonUpMapping(DualShock4Buttons::DPAD_DOWN, OSHelper::FunctionEnum::TRIGGER_DOWN_ARROW);

	// L3 to left mouse button
	AddButtonDownMapping(DualShock4Buttons::L3, OSHelper::FunctionEnum::MOUSE_LEFT_DOWN);
	AddButtonUpMapping(DualShock4Buttons::L3, OSHelper::FunctionEnum::MOUSE_LEFT_UP);

	// R3 to right mouse button
	AddButtonDownMapping(DualShock4Buttons::R3, OSHelper::FunctionEnum::MOUSE_RIGHT_DOWN);
	AddButtonUpMapping(DualShock4Buttons::R3, OSHelper::FunctionEnum::MOUSE_RIGHT_UP);

	// center to center mouse
	AddLongButtonUpMapping(CENTER_TOUCH_BAR, OSHelper::FunctionEnum::CENTER_MOUSE);
}