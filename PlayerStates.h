#ifndef PLAYERSTATES_H
#define PLAYERSTATES_H

#include "Animation.h"
#include "Creature.h"
#include "SDL/include/SDL_rect.h"

class Player;

class PlayerState
{
public:
	PlayerState(Player* player) : player(player) {}
	virtual ~PlayerState() {}
	virtual PlayerState* HandleInput() { return nullptr; }
	virtual update_status Update() { return UPDATE_CONTINUE; }

protected:
	Player* player;
};

class Player_StandState : public PlayerState
{
public:
	Player_StandState(Player* player, const char* staticframe);
	~Player_StandState() {}
	PlayerState* HandleInput();

public:
	SDL_Rect initial_rect = { 0,0,0,0 };
	PlayerState* previous_state = this;
};

class Player_MoveState : public PlayerState
{
public:
	Player_MoveState(Player* player, const char* move_animation, const char* moveup_animation);
	~Player_MoveState() {}
	PlayerState* HandleInput();
	update_status Update();

public:
	Animation moving;
	Animation moving_up;
	Animation* current_animation = nullptr;

};

class Player_JumpState : public PlayerState
{
public:
	Player_JumpState(Player* player, const char* jumpframe);
	~Player_JumpState() {}
	PlayerState* HandleInput();
	update_status Update();
	void SetJumpParameters(int initial_y, Creature::XDirection jump_direction);

private:
	SDL_Rect jumprect = { 0,0,0,0 };
	int speed_y = 2;
	int speed_x = 5;
	float gravity = 1.0f;

	int initial_y = 0;
	Creature::XDirection jump_direction = Creature::XDirection::XIDLE;
	int time = 0;

};
#endif // !PLAYERSTATES_H