#ifndef ENEMYSTATES_H
#define ENEMYSTATES_H

#include "Animation.h"
#include "Creature.h"
#include "SDL/include/SDL_rect.h"

class Enemy;

class EnemyState
{
public:
	EnemyState(Enemy* enemy) : enemy(enemy) {}
	virtual ~EnemyState() {}
	virtual EnemyState* Logic() { return nullptr; }
	virtual update_status Update() { return UPDATE_CONTINUE; }
	virtual void OnExit() {}

protected:
	Enemy* enemy;

};

class Enemy_StandState : public EnemyState
{
public:
	Enemy_StandState(Enemy* enemy, const char* staticframe);
	~Enemy_StandState() {}
	EnemyState* Logic();
	update_status Update();
	void OnExit();

public:
	SDL_Rect initial_rect = { 0,0,0,0 };
	SDL_Rect current_rect = { 0,0,0,0 };
	SDL_Rect current_collider = { 0,0,0,0 };
};

class Enemy_MoveState : public EnemyState
{
public:
	Enemy_MoveState(Enemy* enemy, const char* move_animation, const char* moveup_animation);
	~Enemy_MoveState() {}
	EnemyState* Logic();
	update_status Update();
	void OnExit();

public:
	Animation moving;
	Animation moving_up;
	Animation* current_animation = nullptr;
	SDL_Rect collider_moving = { 0,0,0,0 };
	SDL_Rect collider_moving_up = { 0,0,0,0 };
	SDL_Rect* current_collider = nullptr;
};

class Enemy_JumpState : public EnemyState
{
public:
	Enemy_JumpState(Enemy* enemy, const char* jump_frame, const char* jumpattack_frame);
	~Enemy_JumpState() {}
	EnemyState* Logic();
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

	SDL_Rect jump_collider = { 0,0,0,0 };
	SDL_Rect aerialkick_collider = { 0,0,0,0 };
	SDL_Rect attack_collider = { 0,0,0,0 };
	int kick_damage = 0;
};

class Enemy_AttackState : public EnemyState
{
public:
	Enemy_AttackState(Enemy* enemy, const char* punch_animation, const char* kick_animation = nullptr);
	~Enemy_AttackState() { RELEASE_ARRAY(animation_colliders); }
	EnemyState* Logic();
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
	SDL_Rect* animation_colliders;
	iPoint* shift_frames;
};

class Enemy_DamageState : public EnemyState
{
public:
	Enemy_DamageState(Enemy* enemy, const char* high_frame, const char* low_frame);
	~Enemy_DamageState() { RELEASE(in_damaged_state); }
	EnemyState* Logic() { return this; }
	update_status Update();
	void OnExit();

	void SetFrame(Creature::Attack attack_type)
	{
		switch (attack_type)
		{
		case Creature::PUNCH:
			damage_rect = &high_rect;
			damage_shift = &high_shift;
			break;
		case Creature::KICK:
			damage_rect = &low_rect;
			damage_shift = &low_shift;
			break;
		}
	}

public:
	Timer* in_damaged_state = nullptr;

private:
	SDL_Rect high_rect = { 0,0,0,0 };
	SDL_Rect low_rect = { 0,0,0,0 };
	SDL_Rect* damage_rect = &high_rect;

	iPoint high_shift = { 0,0 };
	iPoint low_shift = { 0,0 };
	iPoint* damage_shift = &high_shift;
};

class Enemy_FallState : public EnemyState
{
private:
	enum FallingState { FALLING, LYING, RISING };

public:
	Enemy_FallState(Enemy* player, const char* falling_frame, const char* fallen_frame, const char* rising_frame);
	~Enemy_FallState() {}
	EnemyState* Logic();
	update_status Update();
	void OnExit();

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

	iPoint fallen_shift = { 0,0 };
	iPoint rising_shift = { 0,0 };
};

#endif // !ENEMYSTATES_H