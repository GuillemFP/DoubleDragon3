#ifndef MODULEUI_H
#define MODULEUI_H

#define MODULE_UI "ModuleUI"
#define UI_SECTION "Config.Modules.UI"

#include <vector>
#include "Module.h"
#include "Point.h"

class Player;
class Timer;
struct TextLine;

class ModuleUI : public Module
{
public:
	ModuleUI(bool active);
	~ModuleUI();

	bool Start();
	bool CleanUp();
	update_status PreUpdate();
	update_status Update();

	int GetLength(int number);
	void IntToString(int number, char* string) { IntToString(number, GetLength(number), string); }
	void IntToString(int number, int final_length, char* string);

private:
	std::vector<Player*> players;
	iPoint* health_pos;
	int* health_fonts;
	iPoint* name_pos;
	iPoint* face_pos;

	const char* time_string;
	iPoint time_name = { 0,0 };
	iPoint time_number = { 0,0 };
	const char* coins_string;
	iPoint coins_name = { 0,0 };
	iPoint coins_number = { 0,0 };
	int name_font;

	char* number_string;

	const char** string_noplayer;
	int num_noplayer_str;
	bool first_lines = true;

	int number_of_players = 0;

	int iPLAYERS_IN_UI = 1;
	int iNUMBER_LENGTH = 4;

	Timer* timer;
};

#endif // !MODULEUI_H