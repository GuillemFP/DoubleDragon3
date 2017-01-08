#ifndef ENTITY_H
#define ENTITY_H

#include <list>
#include "SDL/include/SDL_rect.h"
#include "Timer.h"
#include "Globals.h"
#include "ModuleStages.h"

struct SDL_Texture;
enum update_status;
struct Collider;

class Entity
{
public:
	enum Type
	{
		ROOM,
		PLAYER,
		ENEMY,
		OBJECT,
		UNKNOWN
	};

public:
	Entity(Entity::Type type, SDL_Texture* texture, ModuleStages* stage, Entity* parent = nullptr, bool active = true);
	virtual ~Entity() { RELEASE(timer); }

	virtual update_status PreUpdate() { return UPDATE_CONTINUE; }

	virtual update_status Update();
	virtual bool Draw();

	virtual bool Enable();
	virtual bool Disable();
	virtual bool Delete();

	virtual void HasCollided(Collider* with) {}

	virtual void ChangeParent(Entity* new_parent);

public:
	Point3d position = { 0,0,0 };
	Point3d dimensions = { 0,0,0 };
	Type type = UNKNOWN;
	bool active = true;
	bool to_delete = false;

	std::list<Entity*> contains;

	SDL_Texture* texture = nullptr;
	SDL_Rect entity_rect = { 0,0,0,0 };
	bool inverted_texture = false;

	Timer* timer = nullptr;
	bool has_timer = false;

	Entity* parent = nullptr;
	ModuleStages* stage = nullptr;

	Collider* collider = nullptr;
};

#endif // !ENTITY_H