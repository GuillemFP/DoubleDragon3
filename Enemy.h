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
	enum TargetState { CLOSE, ENGAGED, DETECTED, FAR, TARGETLESS };

	Enemy(const char* name, ModuleStages* stage, Entity* parent);
	~Enemy();
	update_status PreUpdate();
	update_status Update();

	void HasCollided(Collider* with);

	TargetState GetCurrentTargetState() const;
	int DistanceToTarget() const;
	bool SetSlot();
	void FreeSlot();
	bool AttackChoice() const;
	bool TargetWithinAttackRange() const;
	void SetDestinationOnPlayer();
	void SetDestinationInClose();
	void SetDestinationInEngage();
	bool DestinationReached() const;

public:
	Player* current_target = nullptr;
	XDirection target_xdirection;
	ZDirection target_zdirection;
	iPoint* final_destination = &destination;
	Timer* logic_timer = nullptr;
	Timer* attack_timer = nullptr;

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
	int engage_range = 0;
	int closecombate_range = 0;

	bool keeping_distance = false;

private:
	int player_slot = 0;
	iPoint destination = { 0,0 };
	float fattack_time = 0.0f;
};



#endif // !ENEMY_H