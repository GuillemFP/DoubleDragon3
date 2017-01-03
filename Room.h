#ifndef ROOM_H
#define ROOM_H

#include "Entity.h"
#include "SDL/include/SDL_rect.h"

class Room : public Entity
{
public:
	Room(SDL_Texture* texture, const char* name);
	~Room();
	update_status Update();

public:
	SDL_Rect back_rect = { 0,0,0,0 };
};

#endif // !ROOM_H