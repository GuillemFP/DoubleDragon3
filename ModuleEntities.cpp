#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleEntities.h"
#include "ModuleUI.h"
#include "Player.h"
#include "Enemy.h"
#include "Room.h"
#include "Object.h"
#include "JsonHandler.h"
#include "Timer.h"

ModuleEntities::ModuleEntities() : Module(MODULE_ENTITIES)
{
	continue_timer = new Timer();
	stage_timer = new Timer();
}

ModuleEntities::~ModuleEntities()
{
	RELEASE(stage_timer);
	RELEASE(continue_timer);
}

bool ModuleEntities::Start()
{
	bool ret = true;

	LOG("Loading Entities module");

	if (App->parser->LoadObject(ENTITY_SECTION))
	{
		int num_sounds = App->parser->GetInt("NumberOfSounds");
		sounds = new unsigned int[num_sounds];
		App->parser->LoadArrayInObject("Sounds");
		for (int i = 0; i < num_sounds; i++)
			sounds[i] = App->audio->LoadFx(App->parser->GetStringFromArray(i));

		iMAX_PLAYERS = App->parser->GetInt("MaxPlayers");
		initial_coins = App->parser->GetInt("Coins");

		players = App->textures->Load(App->parser->GetString("PlayerTexture"));
		enemies = App->textures->Load(App->parser->GetString("EnemiesTexture"));
		faces = App->textures->Load(App->parser->GetString("PlayerFaces"));
		signals = App->textures->Load(App->parser->GetString("SceneSignals"));

		float continue_time = App->parser->GetFloat("ContinueTime");

		ret = App->parser->UnloadObject();

		if (ret == true)
		{
			continue_timer->SetMaxTime((Uint32)(continue_time*1000.0f));
			coins = initial_coins;
		}
	}

	return ret;
}

bool ModuleEntities::CleanUp()
{
	LOG("Freeing all Entities");

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		RELEASE(*it);
	entities.clear();

	RELEASE_ARRAY(sounds);

	App->textures->Unload(players);
	App->textures->Unload(enemies);
	App->textures->Unload(faces);
	App->textures->Unload(signals);

	return true;
}

update_status ModuleEntities::PreUpdate()
{
	update_status ret = UPDATE_CONTINUE;

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end();)
	{
		if ((*it)->to_delete == true)
		{
			if ((*it)->type == Entity::Type::PLAYER)
				--num_players;
			RELEASE(*it);
			it = entities.erase(it);
		}
		else
			++it;
	}

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->active == true)
			ret = (*it)->PreUpdate();

	return UPDATE_CONTINUE;
}

update_status ModuleEntities::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->active == true)
			ret = (*it)->Update();

	entities.sort(CompareDepth());

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->active == true)
			(*it)->Draw();

	return ret;
}

Entity * ModuleEntities::CreateEntity(Entity::Type type, SDL_Texture* texture, const char* name, ModuleStages* stage, Entity* parent)
{
	static_assert(Entity::Type::UNKNOWN == 4, "code needs update");
	Entity* ret = nullptr;
	switch (type)
	{
	case Entity::ROOM:
		ret = new Room(texture, name, stage);
		break;
	case Entity::PLAYER:
		if (num_players < iMAX_PLAYERS)
		{
			ret = new Player(++num_players, name, stage, parent);
		}
		else
			LOG("Unable to create more players");
		break;
	case Entity::ENEMY:
		ret = new Enemy(name, stage, parent);
		break;
	case Entity::OBJECT:
		ret = new Object(texture, name, stage, parent);
		break;
	case Entity::UNKNOWN:
		break;
	default:
		break;
	}

	if (ret != nullptr)
		entities.push_back(ret);

	return ret;
}

Player* ModuleEntities::GetTargetPlayer(Entity* entity) const
{
	Player* ret = nullptr;

	for (std::list<Entity*>::const_iterator it = entities.begin(); it != entities.end(); ++it)
	{
		if ((*it)->type == Entity::Type::PLAYER)
		{
			Player* player = (Player*)(*it);
			if (player->active)
			{
				if (ret == nullptr)
					ret = player;
				else
				{
					if (DistanceBetweenEntities(entity, (*it)) < DistanceBetweenEntities(entity, (Entity*)ret))
					{
						if (player->assigned_enemies - ret->assigned_enemies < 2)
							ret = player;
					}
					else
					{
						if (ret->assigned_enemies - player->assigned_enemies >= 2)
							ret = player;
					}
				}
			}
		}
	}

	return ret;
}

Player* ModuleEntities::GetPlayerByNumber(int player_num) const
{
	if (player_num < num_players)
	{
		for (std::list<Entity*>::const_iterator it = entities.begin(); it != entities.end(); ++it)
		{
			if ((*it)->type == Entity::Type::PLAYER)
			{
				Player* player = (Player*)(*it);
				if (player->number_player - 1 == player_num)
				{
					return player;
				}
			}
		}
	}

	return nullptr;
}

int ModuleEntities::NumberActivePlayers() const
{
	int ret = 0;
	for (std::list<Entity*>::const_iterator it = entities.begin(); it != entities.end(); ++it)
	{
		if ((*it)->type == Entity::Type::PLAYER && (*it)->active)
			++ret;
	}
	return ret;
}

float ModuleEntities::DistanceBetweenEntities(const Entity* first, const Entity* second) const
{
	return sqrt(Distance2D(first->position.x,first->position.z, second->position.x, second->position.z));
}

float ModuleEntities::Distance2D(int x1, int z1, int x2, int z2) const
{
	float ret = 0.0f;

	ret += (x2 - x1)*(x2 - x1);
	ret += (z2 - z1)*(z2 - z1);

	return sqrt(ret);
}
