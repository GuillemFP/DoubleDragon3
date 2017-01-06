#ifndef OBJECT_H
#define OBJECT_H

#include "Entity.h"
#include "Animation.h"
#include "JsonHandler.h"
#include "Point.h"

class Object : public Entity
{
public:
	Object(SDL_Texture* texture, const char* name, ModuleStages* stage, Entity* parent = nullptr) : Entity(Entity::Type::ROOM, texture, stage, parent)
	{
		if (App->parser->LoadObject(name))
		{
			iPoint position_object = { 0,0 };
			
			has_animation = App->parser->GetBool("Animated");
			if (has_animation == true)
				App->parser->GetAnimation(object_anim, "Animation");
			if (App->parser->GetBool("Timer"))
			{
				timer->SetMaxTime((Uint32)(App->parser->GetFloat("MaxTime") * 1000.0f));
				timer->Start();
			}

			App->parser->GetRect(entity_rect, "Rectangle");
			App->parser->GetPoint(position_object, "RelativePosition");

			if (App->parser->UnloadObject())
			{
				position.x = position_object.x;
				position.y = position_object.y;
				position.z = position_object.y + entity_rect.h;
				if (parent != nullptr)
					position.z += parent->position.z;
				dimensions.x = entity_rect.w;
				dimensions.y = entity_rect.h;
			}
		}
	}

	~Object() {};

	update_status Update()
	{
		if (has_animation == true)
			entity_rect = object_anim.GetCurrentFrame();

		return Entity::Update();
	}

public:
	Animation object_anim;

private:
	bool has_animation = false;
};

#endif // !OBJECT_H