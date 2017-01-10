#ifndef ENEMY_H
#define ENEMY_H

#include "Globals.h"
#include "Entity.h"
#include "Creature.h"
#include "Animation.h"
#include "Point.h"

struct Collider;
class Player;

class EnemyState;
class Enemy_MoveState;
class Enemy_StandState;
class Enemy_JumpState;
class Enemy_AttackState;
class Enemy_DamageState;
class Enemy_FallState;

class Enemy : public Creature
{
public:
	Enemy(const char* name, ModuleStages* stage, Entity* parent);
	~Enemy();
	update_status PreUpdate();
	update_status Update();

	void HasCollided(Collider* with);

	bool TargetWithinSightRange() const;
	bool TargetWithinAttackRange() const;
	void SetDestination();
	bool DestinationReached() const;

public:
	Player* current_target = nullptr;
	XDirection target_xdirection;
	ZDirection target_zdirection;
	iPoint final_destination = { 0,0 };
	Timer* logic_timer = nullptr;

	unsigned int sound_jump = 0;
	unsigned int sound_dying = 0;

	Enemy_MoveState* moving;
	Enemy_StandState* idle;
	Enemy_JumpState* jumping;
	Enemy_AttackState* attacking;
	Enemy_DamageState* damaging;
	Enemy_FallState* falling;

	EnemyState* current_state;

	int attack_range = 0;
	int sight_range = 0;
};



#endif // !ENEMY_H