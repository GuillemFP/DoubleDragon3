#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleSceneStage3.h"
#include "ModuleEntities.h"
#include "ModuleFadeToBlack.h"
#include "Player.h"
#include "Enemy.h"
#include "Room.h"
#include "Object.h"
#include "JsonHandler.h"
#include "ModuleUI.h"
#include <list>

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
	const char* door_string;

	spawn_clock = new Timer();

	if (App->parser->LoadObject(SCENE_SECTION_STAGE3))
	{
		background = App->textures->Load(App->parser->GetString("BackgroundTexture"));
		float music_fade = App->parser->GetFloat("MusicFade");
		const char* music_string = App->parser->GetString("MusicPath");

		const char* scenario = App->parser->GetString("Entity_Scenario_Outside");
		int scenario_detail_num = App->parser->GetInt("Entity_Scenario_Details_Num");
		App->parser->LoadArrayInObject("Entity_Scenario_Details_1");
		std::list<const char*> details;
		for (int i = 0; i < scenario_detail_num; i++)
			details.push_back(App->parser->GetStringFromArray(i));
		door_string = App->parser->GetString("Entity_Scenario_Door");

		iPoint back_dimensions;
		App->parser->GetPoint(back_dimensions, "Dimensions");
		App->parser->GetPoint(plataform_min, "Plataform_Min");
		App->parser->GetPoint(plataform_max, "Plataform_Max");
		plataform_height = App->parser->GetInt("PlataformHeight");

		borders_zmin = new int[back_dimensions.x + 1];
		borders_xmin = new int[back_dimensions.y + 1];
		borders_xmax = new int[back_dimensions.y + 1];
		App->parser->GetIntArray("Borders_zmin", borders_zmin);
		borders_zmax = App->parser->GetInt("Borders_zmax");
		App->parser->GetIntArray("Borders_xmin", borders_xmin);
		App->parser->GetIntArray("Borders_xmax", borders_xmax);

		App->parser->GetPoint(player1_initialpos, "Player1_InitialPos");

		enemies_stage = App->parser->GetInt("NumberOfEnemies");
		enemies_killed = 0;
		max_number_enemies = App->parser->GetInt("MaxEnemies");
		current_enemies = App->parser->GetInt("StartingEnemies");
		iPoint* start_positions = new iPoint[current_enemies];
		App->parser->LoadArrayInObject("StartingEnemiesPositions");
		for (int i = 0; i < current_enemies; i++)
			App->parser->GetPointFromArray(start_positions[i], i);

		float time = App->parser->GetFloat("Timer");

		float spawn_time = App->parser->GetFloat("SpawnTime");
		int num_spawn_points = App->parser->GetInt("NumberSpawnPoints");
		App->parser->LoadArrayInObject("SpawnPoints");
		for (int i = 0; i < num_spawn_points; i++)
		{
			iPoint spawn; 
			App->parser->GetPointFromArray(spawn, i);
			spawn_points.push_back(spawn);
		}

		ret = App->parser->UnloadObject();

		if (ret == true)
		{
			if (App->audio->isPlayingMusic() == false)
				App->audio->PlayMusic(music_string, music_fade);

			outside = (Room*)App->entities->CreateEntity(Entity::ROOM, background, scenario, this);

			for (std::list<const char*>::iterator it = details.begin(); it != details.end(); ++it)
				App->entities->CreateEntity(Entity::OBJECT, background, *it, this, outside);

			for (int i = 0; i < current_enemies; i++)
			{
				Enemy* new_enemy = (Enemy*) App->entities->CreateEntity(Entity::ENEMY, nullptr, ENTITY_SAMURAI, this, outside);
				new_enemy->SetPosition(start_positions[i].x, start_positions[i].y);
			}

			spawn_clock->SetMaxTime((Uint32)(spawn_time*1000.0f));
			spawn_clock->Start();
		}

		App->entities->stage_timer->SetMaxTime((Uint32)(time*1000.0f));
	}

	current_room = outside;

	App->entities->CreateEntity(Entity::OBJECT, App->entities->signals, ENTITY_STORESIGN, this, outside);

	door = (Object*) App->entities->CreateEntity(Entity::OBJECT, background, door_string, this, outside);
	door->Disable();

	player_one = (Player*) App->entities->CreateEntity(Entity::PLAYER, nullptr, ENTITY_PLAYER1, this, current_room);
	player_one->SetPosition(player1_initialpos.x, player1_initialpos.y);

	App->user_interface->Enable();

	App->entities->stage_timer->Start();

	return ret;
}

bool ModuleSceneStage3::CleanUp()
{
	LOG("Unloading Stage3 scene");

	RELEASE_ARRAY(borders_xmax);
	RELEASE_ARRAY(borders_xmin);
	RELEASE_ARRAY(borders_zmin);

	RELEASE(spawn_clock);

	spawn_points.clear();

	current_room->Disable();
	current_room->Delete();

	App->textures->Unload(background);

	ModuleStages::CleanUp();

	return true;
}

update_status ModuleSceneStage3::PreUpdate()
{
	App->renderer->CameraInsideScene(player_one->position.x + player_one->dimensions.x / 2, current_room->position.x, current_room->dimensions.x, current_room->position.y);

	return UPDATE_CONTINUE;
}

update_status ModuleSceneStage3::Update()
{
	ModuleStages::Update();

	if (enemies_killed >= enemies_stage)
		door->Enable();

	SpawnEnemies();

	return UPDATE_CONTINUE;
}

void ModuleSceneStage3::SpawnEnemies()
{
	if (spawn_clock->MaxTimeReached())
	{
		spawn_clock->Reset();
		current_enemies = current_room->GetNumberOfEnemies();
		if (enemies_killed + current_enemies < enemies_stage && current_enemies < max_number_enemies)
		{
			int spawnable_enemies = max_number_enemies - current_enemies;
			for (std::vector<iPoint>::iterator it = spawn_points.begin(); it != spawn_points.end(); ++it)
			{
				if ((*it).x < abs(App->renderer->camera.x / App->window->GetScreenSize()) || (*it).x > abs(App->renderer->camera.x) + App->window->GetScreenWidth())
				{
					Enemy* new_enemy = (Enemy*) App->entities->CreateEntity(Entity::ENEMY, nullptr, ENTITY_SAMURAI, this, outside);
					new_enemy->SetPosition((*it).x, (*it).y);
					--spawnable_enemies;
					if (spawnable_enemies == 0)
						break;
				}
			}
		}
	}
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
