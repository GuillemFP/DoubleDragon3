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

ModuleSceneStage3::ModuleSceneStage3(bool active) : ModuleStages(MODULESCENE_STAGE3, active)
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

		iPoint back_dimensions;
		App->parser->GetPoint(back_dimensions, "Dimensions");
		borders_zmin = new int[back_dimensions.x];
		borders_xmin = new int[back_dimensions.y];
		borders_xmax = new int[back_dimensions.y];
		App->parser->GetIntArray("Borders_zmin", borders_zmin);
		borders_zmax = back_dimensions.y;
		App->parser->GetIntArray("Borders_xmin", borders_xmin);
		App->parser->GetIntArray("Borders_xmax", borders_xmax);

		ret = App->parser->UnloadObject();

		if (ret == true)
		{
			if (App->audio->isPlayingMusic() == false)
				App->audio->PlayMusic(music_string, music_fade);

			outside = (Room*)App->entities->CreateEntity(Entity::ROOM, background, scenario, this);

			for (int i = 0; i < scenario_detail_num; i++)
			{
				App->entities->CreateEntity(Entity::OBJECT, background, details[i], this, outside);
			}
		}
	}

	current_room = outside;

	borders_zmax = current_room->entity_rect.h;

	App->entities->CreateEntity(Entity::OBJECT, signals, ENTITY_STORESIGN, this, current_room);

	player_one = (Player*) App->entities->CreateEntity(Entity::PLAYER, players, ENTITY_PLAYER1, this, current_room);
	player_one->position.x = 50;
	player_one->position.y = 154;
	player_one->position.z = player_one->position.y + player_one->dimensions.y;

	App->entities->CreateEntity(Entity::OBJECT, players, ENTITY_PLAYER1_SIGN, this, player_one);

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
	App->renderer->CameraInsideScene(player_one->position.x + player_one->dimensions.x / 2, current_room->position.x, current_room->dimensions.x);

	return UPDATE_CONTINUE;
}

bool ModuleSceneStage3::InsideScene_LeftBorder(const Point3d& positions, const Point3d& dimensions) const
{
	bool ret = false;

	if (positions.z > 0 && positions.z < current_room->dimensions.y)
		if (positions.x > borders_xmin[positions.z] + 1)
			ret = true;

	return ret;
}

bool ModuleSceneStage3::InsideScene_RightBorder(const Point3d& positions, const Point3d& dimensions) const
{
	bool ret = false;

	if (positions.z > 0 && positions.z < current_room->dimensions.y)
		if (positions.x + dimensions.x < borders_xmax[positions.z] - 1)
			ret = true;

	return ret;
}

bool ModuleSceneStage3::InsideScene_LowBorder(const Point3d& positions, const Point3d& dimensions) const
{
	bool ret = false;

	if (positions.x > 0 && positions.x < current_room->dimensions.x)
		if (positions.z < borders_zmax - 1)
			ret = true;

	return ret;
}

bool ModuleSceneStage3::InsideScene_HighBorder(const Point3d& positions, const Point3d& dimensions) const
{
	bool ret = false;

	if (positions.x > 0 && positions.x < current_room->dimensions.x)
		if (positions.z > borders_zmin[positions.x] + 1 && positions.z > borders_zmin[positions.x + dimensions.x] + 1)
			ret = true;

	return ret;
}
