#include "Application.h"
#include "ModuleRender.h"
#include "Creature.h"
#include "ModuleCollision.h"
#include "JsonHandler.h"

Creature::Creature(Entity::Type type, SDL_Texture* texture, ModuleStages* stage, Entity* parent, bool active) : Entity(type, texture, stage, parent, true) 
{
	immunity_after_attack = new Timer();
	blink = new Timer();
}

Creature::~Creature()
{
	RELEASE(blink);
	RELEASE(immunity_after_attack);
}

bool Creature::Draw()
{
	if (draw == true)
		ShiftedDraw();	

	if (blinking == true)
	{
		if (blink->MaxTimeReached())
		{
			blink->Reset();
			draw = !draw;
		}
	}

	return active;
}

void Creature::Dying()
{
	blink->Reset();
	dead = true;
	blinking = true;
}

void Creature::ShiftedDraw()
{
	if (active && texture != nullptr)
	{
		if (inverted_texture == false)
		{
			if (parent == nullptr)
				App->renderer->Blit(texture, { position.x + shifted_draw.x,position.y + shifted_draw.y }, &entity_rect);
			else
				App->renderer->Blit(texture, { position.x + parent->position.x + shifted_draw.x, position.y + parent->position.y + shifted_draw.y }, &entity_rect);
		}
		else
		{
			if (parent == nullptr)
				App->renderer->Blit(texture, { position.x + dimensions.x - entity_rect.w - shifted_draw.x,position.y + shifted_draw.y }, &entity_rect, true);
			else
				App->renderer->Blit(texture, { position.x + parent->position.x + dimensions.x - entity_rect.w - shifted_draw.x, position.y + parent->position.y + shifted_draw.y }, 
					&entity_rect, true);
		}
	}
}

void Creature::SetPosition(int x, int z)
{ 
	position.x = x; 
	position.z = z; 
	position.y = z - dimensions.y;
	if (stage->InPlataform({position.x, position.z}))
	{
		position.y += stage->plataform_height;
		in_plataform = true;
	}
	else
		in_plataform = false;
	collider->position = position; 
}

void Creature::SetAttackCollider(const SDL_Rect& rect)
{
	if (inverted_texture == true)
		attack_collider->position.x = position.x + dimensions.x - rect.x - rect.w - shifted_draw.x;
	else
		attack_collider->position.x = position.x + rect.x + shifted_draw.x;
	attack_collider->position.y = position.y + rect.y + shifted_draw.y;
	attack_collider->position.z = position.z;
	attack_collider->dimensions.x = rect.w;
	attack_collider->dimensions.y = rect.h;
	attack_collider->dimensions.z = dimensions.z;
}

void Creature::ResetCollider() 
{
	collider->position = position; 
	collider->dimensions = dimensions; 
}

void Creature::ResetColliderPositions() 
{ 
	collider->position = position; 
}

void Creature::ResetShift()
{
	shifted_draw = { 0,0 };
}

void Creature::EnableAttackCollider()
{ 
	if (attack_collider != nullptr) 
		attack_collider->active = true; 
}

void Creature::DisableAttackCollider() 
{ 
	if (attack_collider != nullptr) 
		attack_collider->active = false; 
}

bool Creature::AttackColliderIsActive() const 
{ 
	bool ret = false;
	if (attack_collider != nullptr)
		ret = attack_collider->active;
	return ret;
}

void Creature::SetAttackDamage(int damage) 
{ 
	if (attack_collider != nullptr) 
		attack_collider->damage = damage; 
}

void Creature::SetAttackType(Creature::Attack type) 
{ 
	if (attack_collider != nullptr) 
		attack_collider->attack_type = type; 
}

void Creature::HasCollided(Collider* with)
{
	if (immunity_after_attack->MaxTimeReached())
	{
		accumulated_damage += with->damage;
		if (accumulated_damage >= damage_treshold)
			accumulated_damage -= damage_treshold;
		health -= with->damage;
		if (health < 0)
		{
			health = 0;
			accumulated_damage = 0;
		}
		immunity_after_attack->Reset();
	}
}

Creature::XDirection Creature::RelativeDirectionXTo(int x_coordinate) const
{
	Creature::XDirection ret = XDirection::XIDLE;

	if (position.x < x_coordinate)
		ret = XDirection::RIGHT;
	else if (position.x > x_coordinate)
		ret = XDirection::LEFT;

	return ret;
}

Creature::ZDirection Creature::RelativeDirectionZTo(int z_coordinate) const
{
	Creature::ZDirection ret = ZDirection::YIDLE;

	if (position.z < z_coordinate)
		ret = ZDirection::DOWN;
	else if (position.z > z_coordinate)
		ret = ZDirection::UP;

	return ret;
}
