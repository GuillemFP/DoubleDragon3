#ifndef MODULEENTITIES_H
#define MODULEENTITIES_H

#include "Module.h"
#include "Entity.h"
#include "ModuleStages.h"
#include <list>

#define MODULE_ENTITIES "ModuleEntities"
#define ENTITY_SECTION "Config.Entities.Globals"
#define ENTITY_PLAYER1 "Config.Entities.Player1"
#define ENTITY_PLAYER1_SIGN "Config.Entities.Player1.PlayerSign"

struct SDL_Texture;
class Player;
class Timer;

class ModuleEntities : public Module
{
public:
	struct CompareDepth {
		bool operator()(Entity* lhs, Entity* rhs) { return lhs->position.z < rhs->position.z; }
	};

	ModuleEntities();
	~ModuleEntities();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

	Entity* CreateEntity(Entity::Type type, SDL_Texture* texture, const char* name, ModuleStages* stage, Entity* parent = nullptr);

	unsigned int GetSound(int num_table) { return sounds[num_table]; }
	int GetNumberPlayers() { return num_players; }
	int GetNumberCoins() { return coins; }

	Player* GetPlayerByNumber(int player_num);

public:
	SDL_Texture* players = nullptr;
	SDL_Texture* faces = nullptr;
	SDL_Texture* signals = nullptr;

	Timer* stage_timer;

private:
	std::list<Entity*> entities;
	unsigned int* sounds = nullptr;

	int num_players = 0;
	int coins = 0;

	int iMAX_PLAYERS = 1;
};

#endif // !MODULEENTITIES_H