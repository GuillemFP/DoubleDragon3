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
class Player_DamageState;
class Player_FallState;

class Player : public Creature
{
public:
	Player(int number_player, const char* name, ModuleStages* stage, Entity* parent = nullptr);
	~Player();
	update_status PreUpdate();
	update_status Update();

	void SetPosition(int x, int z) { Creature::SetPosition(x, z); collider_position = position; }

	void HasCollided(Collider* with);
	void ResetCollider() { collider_position = position; collider_dimensions = dimensions; };
	void ResetColliderPositions() { collider_position = position; }

private:
	void HandleInput();

public:
	bool running = false;
	int number_player = 1;
	const char* name;

	unsigned int sound_attack = 0;
	unsigned int sound_jump = 0;

	Player_MoveState* moving;
	Player_StandState* idle;
	Player_JumpState* jumping;
	Player_AttackState* attacking;
	Player_DamageState* damaging;
	Player_FallState* falling;

	PlayerState* current_state = nullptr;

	SDL_Rect face = { 0,0,0,0 };

	Collider* attack_collider;
	Point3d attack_position = { 0,0,0 };	
	Point3d attack_dimensions = { 0,0,0 };

	Point3d collider_position = { 0,0,0 };
	Point3d collider_dimensions = { 0,0,0 };
};

#endif // PLAYER_H