#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleEntities.h"
#include "ModuleFadeToBlack.h"
#include "ModuleUI.h"
#include "ModuleStages.h"
#include "ModuleFonts.h"
#include "ModuleAudio.h"
#include "Player.h"

bool ModuleStages::CleanUp()
{
	game_over = false;
	App->entities->continue_timer->Stop();

	return true;
}

update_status ModuleStages::Update()
{
	bool no_active = false;
	if (App->entities->GetNumberActivePlayers() == 0)
	{
		App->entities->continue_timer->Start();
		if (game_over == false && App->entities->continue_timer->MaxTimeReached())
		{
			App->user_interface->Disable();
			App->entities->continue_timer->Reset();
			game_over = true;
		} 
		else if (game_over == true && (App->entities->continue_timer->MaxTimeReached() || App->input->GetPlayerOutput_KeyDown(1, PlayerOutput::START)) )
		{
			App->entities->continue_timer->Stop();
			App->fade->FadeToBlack((Module*)App->scene_title, this);
			if (App->audio->isPlayingMusic() == true)
				App->audio->StopMusic();
		}
		if (game_over == true)
			App->fonts->BlitScreenCentered(6, "game over");
		no_active = true;
	}
	int num_players = App->entities->GetNumberPlayers();
	if (game_over == false)
	{
		for (int i = 0; i < num_players; i++)
		{
			Player* player = App->entities->GetPlayerByNumber(i);
			if (player->active == false && App->entities->GetNumberCoins() > 0)
			{
				if (App->entities->GetNumberActivePlayers() != 0)
				{
					Player* active_player = App->entities->GetAnActivePlayer();
					player->SetPosition(active_player->position.x, active_player->position.z);
				}
				if (App->input->GetPlayerOutput_KeyDown(i + 1, PlayerOutput::START))
				{
					player->Revive();
					App->entities->SpendCoin();
					if (no_active)
					{
						App->entities->continue_timer->Stop();
						App->entities->stage_timer->Reset();
					}
				}
			}
		}


	}

	return UPDATE_CONTINUE;
}