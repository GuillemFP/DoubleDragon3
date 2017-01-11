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

class Enemy;
class Object;

class Player : public Creature
{
public:
	Player(int number_player, const char* name, ModuleStages* stage, Entity* parent = nullptr);
	~Player();
	update_status PreUpdate();
	update_status Update();

	void UpdateSlots();
	iPoint* GetSlotPosition(int num_slot) const;
	int GetCloserSlot(Enemy* enemy);
	void FreeSlot(int num);
	void FreeAllSlots() { memset(free_slots, true, iMAX_ASSIGNED_ENEMIES); }
	bool EnemyInsideRange(int x, int z) const;
	bool hasFreeSlots() const { return (assigned_enemies < iMAX_ASSIGNED_ENEMIES); }

	void HasCollided(Collider* with);

	void Revive();
	void Rising();
	void SetPosition(int x, int z);

public:
	int number_player = 1;
	const char* name;
	int initial_health = 250;
	Object* sign = nullptr;

	unsigned int sound_attack = 0;
	unsigned int sound_jump = 0;
	unsigned int sound_hit = 0;
	unsigned int sound_falling = 0;
	unsigned int sound_dying = 0;

	Player_MoveState* moving;
	Player_StandState* idle;
	Player_JumpState* jumping;
	Player_AttackState* attacking;
	Player_DamageState* damaging;
	Player_FallState* falling;

	PlayerState* current_state = nullptr;

	SDL_Rect face = { 0,0,0,0 };

	Timer* reviving_timer;
	float blink_time_alive = 0.0f;
	bool reviving = false;
	bool rising = false;

	int assigned_enemies = 0;
	int iMAX_ASSIGNED_ENEMIES = 3;

private:
	int x_range = 20;
	iPoint* fixed_slot_positions = nullptr;
	iPoint** slots = nullptr;
	bool* free_slots = nullptr;
	Timer* slot_change;
	int counter_changes = 0;
};

#endif // PLAYER_H