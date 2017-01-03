#ifndef OBJECT_H
#define OBJECT_H

#include "Entity.h"
#include "SDL/include/SDL_rect.h"

class Object : public Entity
{
public:
	Object(SDL_Texture* texture, const char* name, Entity* parent);
	~Object();
	update_status Update();

public:
	SDL_Rect object_rect = { 0,0,0,0 };
};

#endif // !OBJECT_H