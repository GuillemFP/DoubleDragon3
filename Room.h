#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include "Entity.h"
#include "JsonHandler.h"
#include "Point.h"
#include "Creature.h"

class Room : public Entity
{
public:
	Room(SDL_Texture* texture, const char* name, ModuleStages* stage) : Entity(Entity::Type::ROOM, texture, stage)
	{
		if (App->parser->LoadObject(name))
		{
			iPoint back_position = { 0,0 };
			App->parser->GetRect(entity_rect, "Rectangle");
			App->parser->GetPoint(back_position, "Position");		

			if (App->parser->UnloadObject())
			{
				position.x = back_position.x;
				position.y = back_position.y;
				position.z = -1;
				dimensions.x = entity_rect.w;
				dimensions.y = entity_rect.h;
			}
		}
	}
	~Room() {}

};

#endif // !ROOM_H