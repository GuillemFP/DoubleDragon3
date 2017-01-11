#include "Application.h"
#include "ModuleEntities.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFonts.h"
#include "ModuleCollision.h"
#include "ModuleUI.h"
#include "ModuleDebug.h"
#include "JsonHandler.h"
#include "Player.h"

ModuleDebug::ModuleDebug() : Module(MODULE_DEBUG)
{
	number_string = new char[4];
}

ModuleDebug::~ModuleDebug()
{
	RELEASE_ARRAY(number_string);
}

bool ModuleDebug::Start()
{
	if (App->parser->LoadObject(DEBUG_SECTION))
	{
		number_keys = App->parser->GetInt("FunctionKeyNumber");
		fkey_strings = new const char*[number_keys];
		fkey_positions = new iPoint[number_keys];

		App->parser->LoadArrayInObject("FunctionKeys");
		for (int i = 0; i < number_keys; i++)
			fkey_strings[i] = App->parser->GetStringFromArray(i);
		App->parser->LoadArrayInObject("FunctionPositions");
		for (int i = 0; i < number_keys; i++)
		{
			fkey_positions[i].x = App->parser->GetIntFromArrayInArray(i, 0);
			fkey_positions[i].y = App->parser->GetIntFromArrayInArray(i, 1);
		}

		App->parser->GetPoint(position_letter, "Letter_Init");
		App->parser->GetPoint(position_number, "Number_Init");
		x_increment = App->parser->GetInt("XIncrement");

		unactivated_font = App->parser->GetInt("UnactivatedFontId");
		activated_font = App->parser->GetInt("ActivatedFontId");

		App->parser->UnloadObject();
	}

	return true;
}

bool ModuleDebug::CleanUp()
{
	RELEASE_ARRAY(fkey_positions);
	RELEASE_ARRAY(fkey_strings);

	return true;
}

update_status ModuleDebug::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_F12) == KEY_DOWN)
		bdebug_mode = !bdebug_mode;

	if (bdebug_mode)
	{
		if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
			bdebug_colliders = !bdebug_colliders;
		if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		{
			bdebug_camera = !bdebug_camera;
			App->renderer->bCenterCamera = !App->renderer->bCenterCamera;
		}
		if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
			bdebug_positions = !bdebug_positions;

		if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
		{
			bdebug_god = !bdebug_god;
			int number_players = App->entities->GetNumberPlayers();
			for (int i = 0; i < number_players; i++)
			{
				Player* player = App->entities->GetPlayerByNumber(i);
				if (bdebug_god == true)
					player->immunity_after_attack->SetMaxTime(0);
				else
				{
					player->immunity_after_attack->SetMaxTime((Uint32)(player->immunity_seconds * 1000.0f));
					player->immunity_after_attack->Start();
				}
			}
		}

		if (bdebug_colliders)
		{
			App->collision->DebugDraw();
			App->fonts->Blit(activated_font, fkey_positions[0], fkey_strings[0], 0.0f);
		}
		else
			App->fonts->Blit(unactivated_font, fkey_positions[0], fkey_strings[0], 0.0f);

		if (bdebug_camera)
		{
			App->renderer->DebugCamera();
			App->fonts->Blit(activated_font, fkey_positions[1], fkey_strings[1], 0.0f);
		}
		else
			App->fonts->Blit(unactivated_font, fkey_positions[1], fkey_strings[1], 0.0f);

		if (bdebug_positions)
		{
			int num_players = App->user_interface->GetNumberOfUIs();
			num_players = (num_players > 2) ? 2 : num_players;
			for (int i = 0; i < num_players; i++)
			{
				Player* player = App->entities->GetPlayerByNumber(i);
				App->fonts->Blit(activated_font, position_letter, "x", 0.0f);
				App->fonts->Blit(activated_font, {position_letter.x + x_increment, position_letter.y}, "y", 0.0f);
				App->fonts->Blit(activated_font, { position_letter.x + 2 * x_increment, position_letter.y }, "z", 0.0f);

				App->user_interface->IntToString(player->position.x, number_string);
				App->fonts->BlitFromRight(activated_font, position_number, number_string);
				App->user_interface->IntToString(player->position.y, number_string);
				App->fonts->BlitFromRight(activated_font, { position_number.x + x_increment, position_number.y }, number_string);
				App->user_interface->IntToString(player->position.z, number_string);
				App->fonts->BlitFromRight(activated_font, { position_number.x + 2 * x_increment, position_number.y }, number_string);
			}
			App->fonts->Blit(activated_font, fkey_positions[2], fkey_strings[2], 0.0f);
		}
		else
			App->fonts->Blit(unactivated_font, fkey_positions[2], fkey_strings[2], 0.0f);

		if (bdebug_god)
			App->fonts->Blit(activated_font, fkey_positions[3], fkey_strings[3], 0.0f);
		else
			App->fonts->Blit(unactivated_font, fkey_positions[3], fkey_strings[3], 0.0f);
	}

	return UPDATE_CONTINUE;
}
