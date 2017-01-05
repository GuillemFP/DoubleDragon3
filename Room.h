#ifndef ROOM_H
#define ROOM_H

#include "Entity.h"
#include "JsonHandler.h"
#include "Point.h"

class Room : public Entity
{
public:
	Room(SDL_Texture* texture, const char* name) : Entity(Entity::Type::ROOM, texture)
	{
		if (App->parser->LoadObject(name))
		{
			iPoint position = { 0,0 };
			App->parser->GetRect(entity_rect, "Rectangle");
			App->parser->GetPoint(position, "Position");

			if (App->parser->UnloadObject())
			{
				x = position.x;
				y = position.y;
				z = -1;
				width = entity_rect.w;
				height = entity_rect.h;
			}
		}
	}
	~Room() {};

};

#endif // !ROOM_H