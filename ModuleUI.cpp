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

	players.clear();

	RELEASE_ARRAY(health_pos);
	RELEASE_ARRAY(name_pos);
	RELEASE_ARRAY(face_pos);
	RELEASE_ARRAY(health_fonts);

	RELEASE_ARRAY(number_string);

	return ret;
}

update_status ModuleUI::PreUpdate()
{
	if (players.size() <= iPLAYERS_IN_UI)
	{
		Player* new_player = App->entities->GetPlayerByNumber(players.size() + 1);
		if (new_player != nullptr)
		{
			players.push_back(new_player);
		}
	}

	return UPDATE_CONTINUE;
}

update_status ModuleUI::Update()
{
	
	for (int i = 0; i < iPLAYERS_IN_UI; i++)
	{
		if (i < players.size())
		{
			IntToString(players[i]->health, iNUMBER_LENGTH, number_string);
			App->fonts->BlitFromRight(health_fonts[i], health_pos[i], number_string);
			App->fonts->BlitFromRight(name_font, name_pos[i], players[i]->name);
			App->renderer->Blit(App->entities->faces, face_pos[i], &(players[i]->face), 0.0f);
		}
		else
		{
			if (first_lines == true)
			{
				App->fonts->Blit(name_font, { health_pos[iPLAYERS_IN_UI - 1].x, time_name.y }, string_noplayer[0]);
				App->fonts->Blit(name_font, { health_pos[iPLAYERS_IN_UI - 1].x, coins_name.y }, string_noplayer[1]);
			}
			else
			{
				App->fonts->Blit(name_font, { health_pos[iPLAYERS_IN_UI - 1].x, time_name.y }, string_noplayer[2]);
				App->fonts->Blit(name_font, { health_pos[iPLAYERS_IN_UI - 1].x, coins_name.y }, string_noplayer[3]);
			}
			if (timer->MaxTimeReached())
			{
				timer->Reset();
				first_lines = !first_lines;
			}
		}
	}

	App->fonts->Blit(name_font, time_name, time_string);
	IntToString(App->entities->stage_timer->GetCounterInS(), iNUMBER_LENGTH, number_string);
	App->fonts->BlitFromRight(name_font, time_number, number_string);
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