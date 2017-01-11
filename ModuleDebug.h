#ifndef MODULEDEBUG_H
#define MODULEDEBUG_H

#include "Module.h"
#include "Point.h"

#define MODULE_DEBUG "ModuleDebug"
#define DEBUG_SECTION "Config.Modules.Debug"

class ModuleDebug : public Module
{
public:
	ModuleDebug();
	~ModuleDebug();

	bool Start();
	bool CleanUp();
	update_status Update();

private:
	bool bdebug_mode = false;

	bool bdebug_camera = false;
	bool bdebug_colliders = false;
	bool bdebug_positions = false;
	bool bdebug_god = false;

	char* number_string;

	int unactivated_font;
	int activated_font;

	int number_keys;
	const char** fkey_strings;
	iPoint* fkey_positions;

	iPoint position_letter;
	iPoint position_number;
	int x_increment;
};

#endif // !MODULEDEBUG_H