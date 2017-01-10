#include "Application.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "Entity.h"

Entity::Entity(Entity::Type type, SDL_Texture* texture, ModuleStages* stage, Entity* parent, bool active) : type(type), texture(texture), stage(stage), parent(parent), active(active)
{
	if (parent != nullptr)
		parent->contains.push_back(this);

	timer = new Timer();
}

update_status Entity::Update()
{
	if (timer->MaxTimeReached() == true)
	{
		timer->Stop();
		Disable();
	}

	return UPDATE_CONTINUE;
}

bool Entity::Draw()
{
	if (active && texture != nullptr)
	{
		if (inverted_texture == false)
		{
			if (parent == nullptr)
				App->renderer->Blit(texture, { position.x,position.y }, &entity_rect);
			else
				App->renderer->Blit(texture, { position.x + parent->position.x, position.y + parent->position.y }, &entity_rect);
		}
		else
		{
			if (parent == nullptr)
				App->renderer->Blit(texture, { position.x + dimensions.x - entity_rect.w,position.y }, &entity_rect, true);
			else
				App->renderer->Blit(texture, { position.x + parent->position.x + dimensions.x - entity_rect.w, position.y + parent->position.y }, &entity_rect, true);
		}
	}

	return active;
}

bool Entity::Enable()
{
	if (parent != nullptr && parent->active == false)
	{
		return false;
	}
	else
	{
		if (has_timer == true)
			timer->Start();
		active = true;
		return true;
	}
}

bool Entity::Disable()
{
	bool ret = true;

	if (has_timer == true)
		timer->Stop();

	active = false;
	if (collider != nullptr)
		collider->active = false;

	for (std::list<Entity*>::iterator it = contains.begin(); it != contains.end() && ret == true; ++it)
		ret = (*it)->Disable();

	contains.clear();

	return ret;
}

bool Entity::Delete()
{
	bool ret = true;

	if (has_timer == true)
		timer->Stop();

	if (active == false)
	{
		to_delete = true;
		if (collider != nullptr)
			collider->to_delete = true;
		for (std::list<Entity*>::iterator it = contains.begin(); it != contains.end() && ret == true; ++it)
			ret = (*it)->Delete();
	}

	return ret;
}

void Entity::EnableCollider() 
{ 
	if (collider != nullptr)
		collider->active = true; 
}

void Entity::DisableCollider() 
{ 
	if (collider != nullptr)
		collider->active = false; 
}

bool Entity::ColliderIsActive() const 
{ 
	bool ret = false;
	if (collider != nullptr)
		ret = collider->active; 
	return ret;
}

void Entity::SetCollider(const SDL_Rect& rect)
{
	if (inverted_texture == true)
		collider->position.x = position.x + dimensions.x - rect.x - rect.w - shifted_draw.x;
	else
		collider->position.x = position.x + rect.x + shifted_draw.x;
	collider->position.y = position.y + rect.y + shifted_draw.y;
	collider->position.z = position.z;
	collider->dimensions.x = rect.w;
	collider->dimensions.y = rect.h;
	collider->dimensions.z = dimensions.z;
}

void Entity::ChangeParent(Entity* new_parent)
{
	if (parent != nullptr)
	{
		parent->contains.remove(this);
		parent = new_parent;
		if (new_parent != nullptr)
			parent->contains.push_back(this);
	}
}
