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

	unsigned int GetSound(int num_table) const { return sounds[num_table]; }
	int GetNumberPlayers() const { return num_players; }
	int GetNumberCoins() const { return coins; }
	Player* GetTargetPlayer(Entity* entity) const;

	void ResetCoins() { coins = initial_coins; }
	void SpendCoin() { --coins; }

	Player* GetPlayerByNumber(int player_num) const;
	int NumberActivePlayers() const;
	float DistanceBetweenEntities(const Entity* first, const Entity* second) const;
	float Distance2D(int x1, int z1, int x2, int z2) const;

public:
	SDL_Texture* players = nullptr;
	SDL_Texture* enemies = nullptr;
	SDL_Texture* faces = nullptr;
	SDL_Texture* signals = nullptr;

	Timer* stage_timer;
	Timer* continue_timer;

private:
	std::list<Entity*> entities;
	unsigned int* sounds = nullptr;

	int num_players = 0;
	int coins = 0;
	int initial_coins = 10;

	int iMAX_PLAYERS = 1;
};

#endif // !MODULEENTITIES_H