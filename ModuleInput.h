#ifndef MODULEINPUT_H
#define MODULEINPUT_H

#include "Module.h"
#include "Point.h"
#include "SDL/include/SDL_scancode.h"

#define MODULE_INPUT "ModuleInput"
#define INPUT_SECTION "Config.Modules.Input"

class JSONParser;

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

enum PlayerOutput
{
	GO_UP = 0,
	GO_DOWN,
	GO_LEFT,
	GO_RIGHT,
	START,
	PUNCH,
	JUMP,
	KICK
};

class ModuleInput : public Module
{
public:
	ModuleInput(JSONParser* parser);
	~ModuleInput();

	bool Init();
	bool Start();
	update_status PreUpdate();
	bool CleanUp();

	KeyState GetKey(int id) const;
	KeyState GetMouseButtonDown(int id) const;
	bool GetWindowEvent(EventWindow code) const;
	const iPoint& GetMouseMotion() const;
	const iPoint& GetMousePosition() const;

	bool GetPlayerOutput(int num_player, PlayerOutput input) const;

private:
	bool bwindowEvents[WE_COUNT];
	KeyState* keyboard;
	KeyState* mouse_buttons;
	iPoint mouse_motion;
	iPoint mouse;

	SDL_Scancode* keys_player;
	bool* player_outputs;

	int iMAX_KEYS;
	int iNUM_BUTTONS;
	int iSCREENSIZE;
	int iNUMBERPLAYERS;
	int iKEYS_PLAYER;
	int icount = 0;
};

#endif // !MODULEINPUT_H