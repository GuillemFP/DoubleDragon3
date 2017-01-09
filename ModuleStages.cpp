#include "Application.h"
#include "ModuleInput.h"
#include "ModuleEntities.h"
#include "ModuleFadeToBlack.h"
#include "ModuleUI.h"
#include "ModuleStages.h"
#include "Player.h"

update_status ModuleStages::Update()
{
	bool no_active = false;
	if (App->entities->NumberActivePlayers() == 0)
	{
		App->entities->continue_timer->Start();
		if (App->entities->continue_timer->MaxTimeReached())
		{
			App->fade->FadeToBlack((Module*)App->scene_title, this);
			App->entities->continue_timer->Stop();
			App->user_interface->Disable();
		}
		no_active = true;
	}
	int num_players = App->entities->GetNumberPlayers();
	for (int i = 0; i < num_players; i++)
	{
		Player* player = App->entities->GetPlayerByNumber(i);
		if (player->active == false && App->entities->GetNumberCoins() > 0)
		{
			//if (App->input->GetPlayerOutput_KeyDown(i, PlayerOutput::START))
			if (App->input->GetKey(SDL_SCANCODE_RETURN))
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

	return UPDATE_CONTINUE;
}