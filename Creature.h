#ifndef CREATURE_H
#define CREATURE_H

#include "Entity.h"

class Creature : public Entity
{
public:
	enum XDirection { LEFT = -1, XIDLE = 0, RIGHT = 1 };
	enum ZDirection { UP = -1, YIDLE = 0, DOWN = 1 };
	enum Attack { PUNCH, KICK, NOATTACK };

public:
	Creature(Entity::Type type, SDL_Texture* texture, ModuleStages* stage, Entity* parent = nullptr, bool active = true) : Entity(Entity::Type::PLAYER, texture, stage, parent, true) {}
	~Creature() {}

	void SetPosition(int x, int z) { position.x = x; position.z = z; position.y = z - dimensions.y; }

public:
	XDirection xmovement = XIDLE;
	ZDirection zmovement = YIDLE;
	Attack attack_cmd = NOATTACK;
	bool jump = false;
	int ispeed;

	bool in_plataform = false;

	int health = 1;

};

#endif // !CREATURE_H