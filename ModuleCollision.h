#ifndef MODULECOLLISION_H
#define MODULECOLLISION_H

#include <list>
#include "Module.h"
#include "Globals.h"
#include "Creature.h"

#define MODULE_COLLISION "ModuleCollision"

class Entity;

enum ColliderType { PLAYER, ENEMY, PLAYER_ATTACK, ENEMY_ATTACK };

struct Collider
{
	Point3d* position;
	Point3d* dimensions;
	bool to_delete = false;
	bool active = true;
	ColliderType type;
	Entity* parent = nullptr;

	int damage = 0;
	Creature::Attack attack_type = Creature::Attack::NOATTACK;

	Collider(Point3d* position, Point3d* dimensions, ColliderType type, Entity* parent) : type(type), parent(parent), position(position), dimensions(dimensions) {};

	bool CheckCollision(Collider& collider) const;
};

class ModuleCollision : public Module
{
public:
	ModuleCollision();
	~ModuleCollision();

	update_status PreUpdate();
	update_status Update();

	Collider* AddCollider(Point3d* position, Point3d* dimensions, ColliderType type, Entity* parent);

	void DebugDraw();

private:
	std::list<Collider*> creatures;
	std::list<Collider*> attacks;
	bool debug = false;

};



#endif // !MODULECOLLISION_H