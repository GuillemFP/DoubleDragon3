#ifndef ENTITY_H
#define ENTITY_H

#include <list>
#include "SDL/include/SDL_rect.h"
#include "Timer.h"

struct SDL_Texture;
enum update_status;

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
	Entity(Entity::Type type, SDL_Texture* texture, Entity* parent = nullptr, bool active = true);
	virtual ~Entity() { RELEASE(timer); }

	virtual update_status Update();
	virtual bool Draw();

	virtual bool Enable();
	virtual bool Disable();
	virtual bool Delete();

public:
	int x = 0, y = 0, z = 0;
	int width = 0, height = 0, depth = 0;
	Type type = UNKNOWN;
	bool active = true;
	bool to_delete = false;

	Entity* parent = nullptr;
	std::list<Entity*> contains;

	SDL_Texture* texture = nullptr;
	SDL_Rect entity_rect = { 0,0,0,0 };
	bool inverted_texture = false;

	Timer* timer = nullptr;
	bool has_timer = false;
};

#endif // !ENTITY_H