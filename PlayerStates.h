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
	virtual void OnExit() {}

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
	void OnExit();

public:
	SDL_Rect initial_rect = { 0,0,0,0 };
	SDL_Rect current_rect = { 0,0,0,0 };
	PlayerState* previous_state = this;
};

class Player_MoveState : public PlayerState
{
public:
	Player_MoveState(Player* player, const char* move_animation, const char* moveup_animation);
	~Player_MoveState() {}
	PlayerState* HandleInput();
	update_status Update();
	void OnExit();

public:
	Animation moving;
	Animation moving_up;
	Animation* current_animation = nullptr;
	SDL_Rect last_moving = { 0,0,0,0 };
	SDL_Rect last_moving_up = { 0,0,0,0 };
};

class Player_JumpState : public PlayerState
{
public:
	Player_JumpState(Player* player, const char* jump_frame, const char* jumpattack_frame);
	~Player_JumpState() {}
	PlayerState* HandleInput();
	update_status Update();
	void OnExit();

private:
	void SetJumpParameters();

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

	bool direct_jump = false;
	bool jumping_to_platform = false;
	bool jumping_off_platform = false;
	bool maximum_reached = true;
	bool attacking = false;

	SDL_Rect kick_rect = { 0,0,0,0 };
	int kick_damage = 0;
	iPoint jumpkick_coll_dimensions = { 0,0 };
};

class Player_AttackState : public PlayerState
{
public:
	Player_AttackState(Player* player, const char* punch_animation, const char* kick_animation);
	~Player_AttackState() {}
	PlayerState* HandleInput();
	update_status Update();
	void OnExit();

public:
	Animation punch;
	Animation kick;
	Animation* current_animation = nullptr;

private:
	SDL_Rect punch_rect = { 0,0,0,0 };
	SDL_Rect kick_rect = { 0,0,0,0 };
	int punch_damage = 0;
	int kick_damage = 0;
	int punch_frame = 0;
	int kick_frame = 0;

	bool attacking = false;
	int attack_frame = 0;
	SDL_Rect* attack_rect;

};

class Player_DamageState : public PlayerState
{
public:
	Player_DamageState(Player* player, const char* high_frame, const char* low_frame);
	~Player_DamageState() { RELEASE(in_damaged_state); }
	PlayerState* HandleInput();
	update_status Update();
	void OnExit();

	void SetFrame(Creature::Attack attack_type) 
	{
		switch (attack_type)
		{
		case Creature::PUNCH:
			damage_rect = &high_rect;
			break;
		case Creature::KICK:
			damage_rect = &low_rect;
			break;
		}
	}

private:
	SDL_Rect high_rect = { 0,0,0,0 };
	SDL_Rect low_rect = { 0,0,0,0 };
	SDL_Rect* damage_rect = &high_rect;

	Timer* in_damaged_state = nullptr;
};

class Player_FallState : public PlayerState
{
private:
	enum FallingState { FALLING, LYING, RISING };

public:
	Player_FallState(Player* player, const char* falling_frame, const char* fallen_frame, const char* rising_frame);
	~Player_FallState() {}
	PlayerState* HandleInput();
	update_status Update();
	void OnExit();

	//void InitFall(Creature::XDirection fall_direction, int final_y) { this->fall_direction = fall_direction; this->final_pos.y = final_y; }
	void InitFall(Creature::XDirection fall_direction) { this->fall_direction = fall_direction; }

private:
	void SetFallParameters();

private:
	SDL_Rect falling_rect = { 0,0,0,0 };
	SDL_Rect fallen_rect = { 0,0,0,0 };
	SDL_Rect rising_rect = { 0,0,0,0 };
	fPoint speed = { 0.0,0.0 };
	float gravity = 1.0f;

	iPoint initial_pos = { 0,0 };
	iPoint final_pos = { 0,0 };
	Creature::XDirection fall_direction = Creature::XDirection::XIDLE;
	int time = 0;
	fPoint current_position = { 0.0,0.0 };

	bool jumping_to_platform = false;
	bool jumping_off_platform = false;
	bool maximum_reached = true;

	FallingState state = FALLING;
	Timer* states_timer;

	float fallen_time = 1.0f;
	float rising_time = 1.0f;

	int final_x = 0;
	int fallen_x_shift = 0;
};

#endif // !PLAYERSTATES_H