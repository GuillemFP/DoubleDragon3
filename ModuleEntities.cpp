#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleEntities.h"
#include "Player.h"
#include "Room.h"
#include "Object.h"
#include "JsonHandler.h"

ModuleEntities::ModuleEntities() : Module(MODULE_ENTITIES)
{
}

ModuleEntities::~ModuleEntities()
{
}

bool ModuleEntities::Start()
{
	bool ret = true;

	LOG("Loading Entities module");

	return ret;
}

bool ModuleEntities::CleanUp()
{
	LOG("Freeing all Entities");

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		RELEASE(*it);

	entities.clear();

	return true;
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
		if (num_players == 0)
			ret = new Player(++num_players, texture, name, stage, parent);
		else
			LOG("Unable to create more players");
		break;
	case Entity::ENEMY:
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

update_status ModuleEntities::PreUpdate()
{
	update_status ret = UPDATE_CONTINUE;

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end();)
	{
		if ((*it)->to_delete == true)
		{
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
