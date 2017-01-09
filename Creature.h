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

	virtual void SetPosition(int x, int z) { position.x = x; position.z = z; position.y = z - dimensions.y; }

	void HasCollided(Collider* with);

public:
	XDirection xmovement = XIDLE;
	ZDirection zmovement = YIDLE;
	Attack attack_cmd = NOATTACK;
	bool jump = false;
	int ispeed;

	bool in_plataform = false;

	int health = 1;
	int accumulated_damage = 0;
	int damage_treshold = 0;

	Timer* immunity_after_attack;
	bool dead = false;

protected:
	Timer* blink;
	float blink_ratio = 0.0f;

private:
	bool blinking_on = false;
};

#endif // !CREATURE_H