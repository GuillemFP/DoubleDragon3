#ifndef MODULEINPUT_H
#define MODULEINPUT_H

#include "Module.h"
#include "Point.h"

#define MODULE_INPUT "ModuleInput"
#define NUM_MOUSE_BUTTONS "Config.Modules.Input.MouseButtons"
#define MAX_KEYS "Config.Modules.Input.KeyboardKeys"

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	bool CleanUp();

	KeyState GetKey(int id) const;
	KeyState GetMouseButtonDown(int id) const;
	bool GetWindowEvent(EventWindow code) const;
	const iPoint& GetMouseMotion() const;
	const iPoint& GetMousePosition() const;

private:
	bool bwindowEvents[WE_COUNT];
	KeyState* keyboard;
	KeyState* mouse_buttons;
	iPoint mouse_motion;
	iPoint mouse;

	int iMAX_KEYS;
	int iNUM_BUTTONS;
};

#endif // !MODULEINPUT_H