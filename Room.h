#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include "Entity.h"
#include "ModuleCollision.h"
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

			has_transition = App->parser->GetBool("HasTransition");
			if (has_transition)
			{
				Point3d collider_pos;
				Point3d collider_dimensions;
				App->parser->GetPoint3D(collider_pos, "TransitionColliderPosition");
				App->parser->GetPoint3D(collider_dimensions, "TransitionColliderDimensions");
				collider = App->collision->AddCollider(collider_pos, collider_dimensions, ColliderType::ACTIVATION, this);
				collider->active = false;
			}

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

	int GetNumberOfEnemies() const
	{
		int ret = 0;
		for (std::list<Entity*>::const_iterator it = contains.begin(); it != contains.end(); ++it)
		{
			if ((*it)->type == Entity::ENEMY)
				++ret;
		}
		return ret;
	}

	void HasCollided(Collider* with)
	{
		if (stage->transition == false)
			stage->transition = true;
	}

public:
	bool has_transition = false;
};

#endif // !ROOM_H