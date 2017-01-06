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

public:
	XDirection xmovement = XIDLE;
	ZDirection zmovement = YIDLE;
	Attack attack_cmd = NOATTACK;
	bool jump = false;
	int ispeed;

	bool binside_x = true;
	bool binside_z = true;

};

#endif // !CREATURE_H