#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleEntities.h"
#include "ModuleFonts.h"
#include "ModuleUI.h"
#include "Player.h"
#include "JsonHandler.h"
#include "Timer.h"


ModuleUI::ModuleUI(bool active) : Module(MODULE_UI, active)
{
}

ModuleUI::~ModuleUI()
{
}

bool ModuleUI::Start()
{
	bool ret = true;

	LOG("Loading user interface module");

	if (App->parser->LoadObject(UI_SECTION))
	{
		iPLAYERS_IN_UI = App->parser->GetInt("PlayersInUI");
		iNUMBER_LENGTH = App->parser->GetInt("NumberLength");
		health_pos = new iPoint[iPLAYERS_IN_UI];
		name_pos = new iPoint[iPLAYERS_IN_UI];
		face_pos = new iPoint[iPLAYERS_IN_UI];
		health_fonts = new int[iPLAYERS_IN_UI];

		App->parser->LoadArrayInObject("Health_RelPos");
		for (int i = 0; i < iPLAYERS_IN_UI; i++)
		{
			health_pos[i].x = App->parser->GetIntFromArrayInArray(i, 0);
			health_pos[i].y = App->parser->GetIntFromArrayInArray(i, 1);
		}
		App->parser->LoadArrayInObject("Name_RelPos");
		for (int i = 0; i < iPLAYERS_IN_UI; i++)
		{
			name_pos[i].x = App->parser->GetIntFromArrayInArray(i, 0);
			name_pos[i].y = App->parser->GetIntFromArrayInArray(i, 1);
		}
		App->parser->LoadArrayInObject("Face_RelPos");
		for (int i = 0; i < iPLAYERS_IN_UI; i++)
		{
			face_pos[i].x = App->parser->GetIntFromArrayInArray(i, 0);
			face_pos[i].y = App->parser->GetIntFromArrayInArray(i, 1);
		}
		App->parser->LoadArrayInObject("Health_Font");
		for (int i = 0; i < iPLAYERS_IN_UI; i++)
			health_fonts[i] = App->parser->GetIntFromArray(i);

		time_string = App->parser->GetString("TimeName");
		App->parser->GetPoint(time_name, "TimeName_RelPos");
		App->parser->GetPoint(time_number, "TimeNumber_RelPos");
		coins_string = App->parser->GetString("CoinsName");
		App->parser->GetPoint(coins_name, "CoinsName_RelPos");
		App->parser->GetPoint(coins_number, "CoinsNumber_RelPos");
		name_font = App->parser->GetInt("Name_Font");

		float press_start = App->parser->GetFloat("PressStartTimer");

		num_noplayer_str = App->parser->GetInt("NumTextLines");
		string_noplayer = new const char*[num_noplayer_str];
		App->parser->LoadArrayInObject("TextLines");
		for (int i = 0; i < num_noplayer_str; i++)
			string_noplayer[i] = App->parser->GetStringFromArray(i);
		string_deadplayer = new const char*[num_noplayer_str];
		App->parser->LoadArrayInObject("TextLines2");
		for (int i = 0; i < num_noplayer_str; i++)
			string_deadplayer[i] = App->parser->GetStringFromArray(i);

		ret = App->parser->UnloadObject();

		if (ret == true)
		{
			int half_width = App->window->GetScreenWidth() / 2;
			time_name.x += half_width;
			time_number.x += half_width;
			coins_name.x += half_width;
			coins_number.x += half_width;
			for (int i = 0; i < iPLAYERS_IN_UI; i++)
			{
				health_pos[i].x += half_width;
				name_pos[i].x += half_width;
				face_pos[i].x += half_width;
			}

			timer = new Timer((Uint32)(press_start * 1000.0f));
		}
	}

	number_string = new char[4];

	timer->Start();

	return ret;
}

bool ModuleUI::CleanUp()
{
	bool ret = true;

	LOG("Freeing user interface");

	//players.clear();

	RELEASE_ARRAY(number_string);
	RELEASE(timer);

	RELEASE_ARRAY(health_pos);
	RELEASE_ARRAY(name_pos);
	RELEASE_ARRAY(face_pos);
	RELEASE_ARRAY(health_fonts);

	RELEASE_ARRAY(string_noplayer);
	RELEASE_ARRAY(string_deadplayer);

	return ret;
}

update_status ModuleUI::Update()
{	
	bool no_players_alive = false;

	if (timer->MaxTimeReached())
	{
		timer->Reset();
		first_lines = !first_lines;
	}

	if (App->entities->NumberActivePlayers() == 0)
	{
		PrintStrings(0, first_lines, string_deadplayer);
		for (int i = 1; i < iPLAYERS_IN_UI; i++)
			PrintStrings(i, first_lines, string_noplayer);
		IntToString(App->entities->continue_timer->GetCounterInS(), iNUMBER_LENGTH, number_string);
		App->fonts->BlitFromRight(name_font, time_number, number_string);
	}
	else
	{
		for (int i = 0; i < iPLAYERS_IN_UI; i++)
		{
			if (i < App->entities->GetNumberPlayers())
			{
				Player* player = App->entities->GetPlayerByNumber(i);
				if (player->active)
				{
					IntToString(player->health, iNUMBER_LENGTH, number_string);
					App->fonts->BlitFromRight(health_fonts[i], health_pos[i], number_string);
					App->fonts->BlitFromRight(name_font, name_pos[i], player->name);
					App->renderer->Blit(App->entities->faces, face_pos[i], &(player->face), 0.0f);
				}
				else
					PrintStrings(i, first_lines, string_noplayer);
			}
			/*else
				PrintStrings(iPLAYERS_IN_UI - 1, first_lines, string_noplayer);*/
		}
		IntToString(App->entities->stage_timer->GetCounterInS(), iNUMBER_LENGTH, number_string);
		App->fonts->BlitFromRight(name_font, time_number, number_string);
	}

	App->fonts->Blit(name_font, time_name, time_string);
	App->fonts->Blit(name_font, coins_name, coins_string);
	IntToString(App->entities->GetNumberCoins(), iNUMBER_LENGTH - 1, number_string);
	App->fonts->BlitFromRight(name_font, coins_number, number_string);

	return UPDATE_CONTINUE;
}

int ModuleUI::GetLength(int number)
{
	int n = number;
	int length = 0;
	do
	{
		++length;
		n /= 10;
	} while (n);

	return length;
}

void ModuleUI::IntToString(int number, int final_length, char* string)
{
	int intlength = GetLength(number);

	for (int i = 0; i < final_length - intlength; i++)
		string[i] = '0';
	sprintf(&string[final_length - intlength], "%d", number);
	
}

void ModuleUI::PrintStrings(int player, bool first_lines, const char** string_array)
{
	if (player % 2 == 0)
	{
		if (first_lines == true)
		{
			App->fonts->BlitFromRight(name_font, { health_pos[player].x, time_name.y }, string_array[0]);
			App->fonts->BlitFromRight(name_font, { health_pos[player].x, coins_name.y }, string_array[1]);
		}
		else
		{
			App->fonts->BlitFromRight(name_font, { health_pos[player].x, time_name.y }, string_array[2]);
			App->fonts->BlitFromRight(name_font, { health_pos[player].x, coins_name.y }, string_array[3]);
		}
	}
	else
	{
		if (first_lines == true)
		{
			App->fonts->Blit(name_font, { health_pos[player].x, time_name.y }, string_array[0]);
			App->fonts->Blit(name_font, { health_pos[player].x, coins_name.y }, string_array[1]);
		}
		else
		{
			App->fonts->Blit(name_font, { health_pos[player].x, time_name.y }, string_array[2]);
			App->fonts->Blit(name_font, { health_pos[player].x, coins_name.y }, string_array[3]);
		}
	}
}

int ModuleUI::GetNumberOfUIs() const 
{
	int number_players = App->entities->GetNumberPlayers();

	return ((number_players > iPLAYERS_IN_UI) ? iPLAYERS_IN_UI : number_players);
}