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
	update_status Update();

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
	Player_JumpState(Player* player, const char* jump_frame, const char* jumpattack_frame);
	~Player_JumpState() {}
	PlayerState* HandleInput();
	update_status Update();
	void SetJumpParameters(Creature::XDirection jump_direction);

private:
	SDL_Rect jump_rect = { 0,0,0,0 };
	SDL_Rect jumpkick_rect = { 0,0,0,0 };
	fPoint speed = { 0.0,0.0 };
	float gravity = 1.0f;

	iPoint initial_pos = { 0,0 };
	iPoint final_pos = { 0,0 };
	Creature::XDirection jump_direction = Creature::XDirection::XIDLE;
	int time = 0;
	fPoint current_position = { 0.0,0.0 };

	bool jumping_to_platform = false;
	bool jumping_off_platform = false;
	bool maximum_reached = true;
	bool attacking = false;

};

class Player_AttackState : public PlayerState
{
public:
	Player_AttackState(Player* player, const char* punch_animation, const char* kick_animation);
	~Player_AttackState() {}
	PlayerState* HandleInput() { return this; }
	update_status Update();

public:
	Animation punch;
	Animation kick;
	Animation* current_animation = nullptr;
};
#endif // !PLAYERSTATES_H