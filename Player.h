#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Creature.h"
#include "Animation.h"

class PlayerState;
class Player_MoveState;
class Player_StandState;
class Player_JumpState;

class Player : public Creature
{
public:
	Player(int number_player, SDL_Texture* texture, const char* name, ModuleStages* stage, Entity* parent = nullptr);
	~Player();
	update_status PreUpdate();
	update_status Update();

	//bool InsideBordersX();
	//bool InsideBordersZ();

private:
	void HandleInput();

public:
	bool running = false;
	int number_player = 1;

	ZDirection last_zmov = YIDLE;

	Player_MoveState* moving;
	Player_StandState* idle;
	Player_JumpState* jumping;
	PlayerState* current_state;
};

#endif // PLAYER_H