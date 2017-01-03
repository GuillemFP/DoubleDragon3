#ifndef ENTITY_H
#define ENTITY_H

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
	Entity(Entity::Type type, SDL_Texture* texture, Entity* parent = nullptr) : type(type), texture(texture), parent(parent), active(true) {}
	virtual ~Entity() {}
	virtual update_status PreUpdate() { return UPDATE_CONTINUE; }
	virtual update_status Update() { return UPDATE_CONTINUE; }

public:
	int x = 0, y = 0, z = 0;
	int width = 0, height = 0, depth = 0;
	Type type = UNKNOWN;
	bool active = true;
	bool to_delete = false;
	Entity* parent;

	SDL_Texture* texture;
};

#endif // !ENTITY_H