#ifndef CREATURE_H
#define CREATURE_H

#include "Entity.h"
#include "Timer.h"

struct Collider;

class Creature : public Entity
{
public:
	enum XDirection { LEFT = -1, XIDLE = 0, RIGHT = 1 };
	enum ZDirection { UP = -1, YIDLE = 0, DOWN = 1 };
	enum Attack { PUNCH, KICK, NOATTACK };

public:
	Creature(Entity::Type type, SDL_Texture* texture, ModuleStages* stage, Entity* parent = nullptr, bool active = true);
	~Creature();

	bool Draw();

	void Dying();

	virtual void SetPosition(int x, int z);
	void ResetShift();

	void SetAttackCollider(const SDL_Rect& rect);
	void ResetCollider();
	void ResetColliderPositions();

	void EnableAttackCollider();
	void DisableAttackCollider();
	bool AttackColliderIsActive() const;
	void SetAttackDamage(int damage);
	void SetAttackType(Creature::Attack type);

	void HasCollided(Collider* with);

	Creature::XDirection RelativeDirectionXTo(Creature* target) const { return RelativeDirectionXTo(target->position.x); }
	Creature::ZDirection RelativeDirectionZTo(Creature* target) const { return RelativeDirectionZTo(target->position.z); }
	Creature::XDirection RelativeDirectionXTo(int x_coordinate) const;
	Creature::ZDirection RelativeDirectionZTo(int z_coordinate) const;

protected:
	void ShiftedDraw();

public:
	XDirection xmovement = XIDLE;
	ZDirection zmovement = YIDLE;
	Attack attack_cmd = NOATTACK;
	bool jump = false;
	bool running = false;
	int ispeed;

	bool in_plataform = false;

	int health = 1;
	int accumulated_damage = 0;
	int damage_treshold = 0;

	Timer* immunity_after_attack;
	bool dead = false;
	bool draw = true;
	bool blinking = false;

protected:
	Timer* blink;
	float blink_ratio = 0.0f;
	Collider* attack_collider;
};

#endif // !CREATURE_H