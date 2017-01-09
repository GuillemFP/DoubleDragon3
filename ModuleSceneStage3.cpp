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

#include "ModuleUI.h"

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
	iPoint player1_initialpos = { 0,0 };

	if (App->parser->LoadObject(SCENE_SECTION_STAGE3))
	{
		background = App->textures->Load(App->parser->GetString("BackgroundTexture"));
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
		App->parser->GetPoint(plataform_min, "Plataform_Min");
		App->parser->GetPoint(plataform_max, "Plataform_Max");
		plataform_height = App->parser->GetInt("PlataformHeight");

		borders_zmin = new int[back_dimensions.x + 1];
		borders_xmin = new int[back_dimensions.y + 1];
		borders_xmax = new int[back_dimensions.y + 1];
		App->parser->GetIntArray("Borders_zmin", borders_zmin);
		borders_zmax = back_dimensions.y;
		App->parser->GetIntArray("Borders_xmin", borders_xmin);
		App->parser->GetIntArray("Borders_xmax", borders_xmax);

		App->parser->GetPoint(player1_initialpos, "Player1_InitialPos");

		float time = App->parser->GetFloat("Timer");

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

		App->entities->stage_timer->SetMaxTime((Uint32)(time*1000.0f));
	}

	current_room = outside;

	App->entities->CreateEntity(Entity::OBJECT, App->entities->signals, ENTITY_STORESIGN, this, current_room);

	player_one = (Player*) App->entities->CreateEntity(Entity::PLAYER, nullptr, ENTITY_PLAYER1, this, current_room);
	player_one->SetPosition(player1_initialpos.x, player1_initialpos.y);

	App->entities->CreateEntity(Entity::OBJECT, App->entities->signals, ENTITY_PLAYER1_SIGN, this, player_one);

	App->user_interface->Enable();

	App->entities->stage_timer->Start();

	return ret;
}

bool ModuleSceneStage3::CleanUp()
{
	LOG("Unloading Stage3 scene");

	current_room->Disable();

	App->textures->Unload(background);

	return true;
}

update_status ModuleSceneStage3::PreUpdate()
{
	App->renderer->CameraInsideScene(player_one->position.x + player_one->dimensions.x / 2, current_room->position.x, current_room->dimensions.x, current_room->position.y);

	return UPDATE_CONTINUE;
}

bool ModuleSceneStage3::InsideScene_LeftBorder(const Point3d& positions, const Point3d& dimensions, bool in_plataform) const
{
	bool ret = false;

	if (in_plataform == true)
	{
		if (positions.x > plataform_min.x)
			ret = true;
	}
	else
		if (positions.x > borders_xmin[positions.z])
			ret = true;

	return ret;
}

bool ModuleSceneStage3::InsideScene_RightBorder(const Point3d& positions, const Point3d& dimensions, bool in_plataform) const
{
	bool ret = false;

	if (in_plataform == true)
	{
		if (positions.x + dimensions.x < plataform_max.x)
			ret = true;
	}
	else
	{
		if (positions.x + dimensions.x < borders_xmax[positions.z])
			ret = true;
	}
	

	return ret;
}

bool ModuleSceneStage3::InsideScene_LowBorder(const Point3d& positions, const Point3d& dimensions, bool in_plataform) const
{
	bool ret = false;

	if (in_plataform == true)
	{
		if (positions.z < plataform_max.y)
			ret = true;
	}
	else
	{
		if (positions.z < borders_zmax)
			ret = true;
	}

	return ret;
}

bool ModuleSceneStage3::InsideScene_HighBorder(const Point3d& positions, const Point3d& dimensions, bool in_plataform) const
{
	bool ret = false;

	if (in_plataform == true)
	{
		if (positions.z > plataform_min.y)
			ret = true;
	}
	else
	{
		if (positions.z > borders_zmin[positions.x] && positions.z > borders_zmin[positions.x + dimensions.x])
			ret = true;
	}

	return ret;
}

bool ModuleSceneStage3::InPlataform(int x, int z) const
{
	bool ret = false;

	if (x > plataform_min.x && x <= plataform_max.x)
		if (z >= plataform_min.y && z < plataform_max.y)
			ret = true;

	return ret;
}
