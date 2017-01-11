#ifndef MODULECOLLISION_H
#define MODULECOLLISION_H

#include <list>
#include "Module.h"
#include "Globals.h"
#include "Creature.h"

#define MODULE_COLLISION "ModuleCollision"

class Entity;

enum ColliderType { PLAYER = 0, ENEMY, PLAYER_ATTACK, ENEMY_ATTACK, ACTIVATION };

const bool ColliderMatrix[5][5] = { { false,false,false,true, true },{ false,false,true,false, false },{ false,true,false,false, false },{ true,false,false,false, false }, {true, false,false,false,false } };

struct Collider
{
	Point3d position;
	Point3d dimensions;
	bool to_delete = false;
	bool active = true;
	ColliderType type;
	Entity* parent = nullptr;

	int damage = 0;
	Creature::Attack attack_type = Creature::Attack::NOATTACK;

	Collider(const Point3d& position, const Point3d& dimensions, ColliderType type, Entity* parent) : type(type), parent(parent), position(position), dimensions(dimensions) {};

	bool CheckCollision(const Collider& collider) const;
};

class ModuleCollision : public Module
{
public:
	ModuleCollision();
	~ModuleCollision();

	update_status PreUpdate();
	update_status Update();

	Collider* AddCollider(const Point3d& position, const Point3d& dimensions, ColliderType type, Entity* parent);

	void DebugDraw() const;

private:
	std::list<Collider*> creatures;
	std::list<Collider*> attacks;
	std::list<Collider*> activations;

};



#endif // !MODULECOLLISION_H