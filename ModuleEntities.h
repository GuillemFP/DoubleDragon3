#ifndef MODULEENTITIES_H
#define MODULEENTITIES_H

#include "Module.h"
#include "Entity.h"
#include <list>

#define MODULE_ENTITIES "ModuleEntities"
#define ENTITY_SECTION "Config.Entities.Globals"
#define ENTITY_PLAYER1 "Config.Entities.Player1"

struct SDL_Texture;

class ModuleEntities : public Module
{
public:
	struct CompareDepth {
		bool operator()(Entity* lhs, Entity* rhs) { return lhs->z < rhs->z; }
	};

	ModuleEntities();
	~ModuleEntities();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

	Entity* CreateEntity(Entity::Type type, SDL_Texture* texture, const char* name, Entity* parent = nullptr);

public:
	SDL_Texture* background = nullptr;
	SDL_Texture* players = nullptr;

private:
	std::list<Entity*> entities;

	int num_players = 0;
};

#endif // !MODULEENTITIES_H