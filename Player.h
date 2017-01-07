#ifndef PLAYER_H
#define PLAYER_H

#include "Globals.h"
#include "Entity.h"
#include "Creature.h"
#include "Animation.h"

class PlayerState;
class Player_MoveState;
class Player_StandState;
class Player_JumpState;
class Player_AttackState;

class Player : public Creature
{
public:
	Player(int number_player, SDL_Texture* texture, const char* name, ModuleStages* stage, Entity* parent = nullptr);
	~Player();
	update_status PreUpdate();
	update_status Update();

private:
	void HandleInput();

public:
	bool running = false;
	int number_player = 1;

	unsigned int sound_attack = 0;
	unsigned int sound_jump = 0;

	ZDirection last_zmov = YIDLE;

	Player_MoveState* moving;
	Player_StandState* idle;
	Player_JumpState* jumping;
	Player_AttackState* attacking;
	PlayerState* current_state;
};

#endif // PLAYER_H