#ifndef OBJECT_H
#define OBJECT_H

#include "Application.h"
#include "ModuleCollision.h"
#include "Entity.h"
#include "Animation.h"
#include "JsonHandler.h"
#include "Point.h"

class Object : public Entity
{
public:
	Object(SDL_Texture* texture, const char* name, ModuleStages* stage, Entity* parent = nullptr) : Entity(Entity::Type::OBJECT, texture, stage, parent)
	{
		if (App->parser->LoadObject(name))
		{
			iPoint position_object = { 0,0 };
			
			has_animation = App->parser->GetBool("Animated");
			if (has_animation == true)
			{
				App->parser->GetAnimation(object_anim, "Animation");
				animation_active = true;
			}

			has_timer = App->parser->GetBool("Timer");
			if (has_timer)
			{
				timer->SetMaxTime((Uint32)(App->parser->GetFloat("MaxTime") * 1000.0f));
				timer->Start();
			}

			App->parser->GetRect(entity_rect, "Rectangle");
			App->parser->GetPoint(position_object, "RelativePosition");

			has_activation = App->parser->GetBool("HasActivation");
			only_one = App->parser->GetBool("OnlyTriggerOnce");
			bool in_plataform = App->parser->GetBool("InPlataform");
			animation_stops = App->parser->GetBool("AnimationStops");

			if (has_activation)
			{
				animation_active = false;
				Point3d collider_pos;
				Point3d collider_dimensions;
				App->parser->GetPoint3D(collider_pos, "ActivationColliderPosition");
				App->parser->GetPoint3D(collider_dimensions, "ActivationColliderDimensions");
				collider = App->collision->AddCollider(collider_pos, collider_dimensions, ColliderType::ACTIVATION, this);
			}

			has_attack = App->parser->GetBool("HasAttack");
			if (has_attack)
			{
				attack_collider = App->collision->AddCollider(position, dimensions, ColliderType::ENEMY_ATTACK, this);
				attack_collider->active = false;
				attack_collider->attack_type = Creature::Attack::PUNCH;
				attack_collider->damage = App->parser->GetInt("TrapDamage");
				min_frame_damage = App->parser->GetInt("MinFrameDamage");
				max_frame_damage = App->parser->GetInt("MaxFrameDamage");
			}

			bool different_height = App->parser->GetBool("HasDifferentHeight");
			int height_shift = 0;
			if (different_height)
				height_shift = App->parser->GetInt("Height");

			if (App->parser->UnloadObject())
			{
				position.x = position_object.x;
				position.y = position_object.y;
				position.z = position_object.y + entity_rect.h + height_shift;
				if (in_plataform)
					position.z += stage->plataform_height;
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
		if (has_animation == true && animation_active == true)
		{
			if (only_one == false || triggered == false)
			{
				entity_rect = object_anim.GetCurrentFrame();
				int frame_num = object_anim.GetCurrentFrameNumber();
				if (attack_collider != nullptr)
				{
					if (frame_num >= min_frame_damage && frame_num <= max_frame_damage)
					{
						attack_collider->active = true;
						attack_collider->position = position;
						attack_collider->dimensions.x = entity_rect.w;
						attack_collider->dimensions.y = entity_rect.h;
						attack_collider->dimensions.z = entity_rect.h;
					}
					else
						attack_collider->active = false;
				}
				if (object_anim.Finished() && animation_stops)
				{
					object_anim.Reset();
					animation_active = false;
					triggered = true;
				}
			}
		}

		return Entity::Update();
	}

	void HasCollided(Collider* with) 
	{
		if (animation_active == false)
			animation_active = true;
	}

public:
	Animation object_anim;
	Collider* attack_collider = nullptr;

private:
	bool has_animation = false;
	bool animation_active = false;
	bool has_activation = false;
	bool animation_stops = false;

	bool only_one = false;
	bool triggered = false;

	bool has_attack = false;
	int min_frame_damage = 0;
	int max_frame_damage = 0;
};

#endif // !OBJECT_H