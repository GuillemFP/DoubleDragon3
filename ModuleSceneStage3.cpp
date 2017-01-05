#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleSceneStage3.h"
#include "ModuleEntities.h"
#include "Player.h"
#include "Room.h"
#include "JsonHandler.h"

ModuleSceneStage3::ModuleSceneStage3(bool active) : Module(MODULESCENE_STAGE3, active)
{
}

ModuleSceneStage3::~ModuleSceneStage3()
{
}

bool ModuleSceneStage3::Start()
{
	bool ret = true;

	LOG("Loading Stage3 scene");

	App->renderer->camera.x = App->renderer->camera.y = 0;

	if (App->parser->LoadObject(SCENE_SECTION_STAGE3))
	{
		background = App->textures->Load(App->parser->GetString("BackgroundTexture"));
		players = App->textures->Load(App->parser->GetString("PlayerTexture"));
		signals = App->textures->Load(App->parser->GetString("SceneSignals"));
		float music_fade = App->parser->GetFloat("MusicFade");
		const char* music_string = App->parser->GetString("MusicPath");

		const char* scenario = App->parser->GetString("Entity_Scenario_Outside");
		int scenario_detail_num = App->parser->GetInt("Entity_Scenario_Details_Num");
		App->parser->LoadArrayInObject("Entity_Scenario_Details_1");
		const char** details = new const char*[scenario_detail_num];
		for (int i = 0; i < scenario_detail_num; i++)
		{
			details[i] = App->parser->GetStringFromArray(i);
		}

		ret = App->parser->UnloadObject();

		if (ret == true)
		{
			if (App->audio->isPlayingMusic() == false)
				App->audio->PlayMusic(music_string, music_fade);

			outside = (Room*)App->entities->CreateEntity(Entity::ROOM, background, scenario);

			for (int i = 0; i < scenario_detail_num; i++)
			{
				App->entities->CreateEntity(Entity::OBJECT, background, details[i], outside);
			}
		}
	}

	current_room = outside;

	App->entities->CreateEntity(Entity::OBJECT, signals, ENTITY_STORESIGN);

	player_one = (Player*) App->entities->CreateEntity(Entity::PLAYER, players, ENTITY_PLAYER1);
	player_one->x = 50;
	player_one->y = 154;
	player_one->z = player_one->y + player_one->height;

	App->entities->CreateEntity(Entity::OBJECT, players, ENTITY_PLAYER1_SIGN, player_one);

	return ret;
}

bool ModuleSceneStage3::CleanUp()
{
	LOG("Unloading Stage3 scene");

	current_room->Disable();

	App->textures->Unload(background);
	App->textures->Unload(players);
	App->textures->Unload(signals);

	return true;
}

update_status ModuleSceneStage3::Update()
{
	App->renderer->CameraInsideScene(player_one->x + player_one->width / 2, current_room->x, current_room->width);

	return UPDATE_CONTINUE;
}
