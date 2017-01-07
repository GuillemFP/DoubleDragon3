#include "Application.h"
#include "ModuleRender.h"
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
	for (std::list<Entity*>::iterator it = contains.begin(); it != contains.end() && ret == true; ++it)
		ret = (*it)->Disable();

	return ret;
}

bool Entity::Delete()
{
	if (has_timer == true)
		timer->Stop();

	if (active == false)
	{
		to_delete = true;
		for (std::list<Entity*>::iterator it = contains.begin(); it != contains.end(); ++it)
		{
			(*it)->active = false;
			(*it)->to_delete = true;
		}

	}

	return to_delete;
}

void Entity::ChangeParent(Entity* new_parent)
{
	if (parent != nullptr)
	{
		parent->contains.remove(this);
		if (new_parent != nullptr)
		{
			parent = new_parent;
			parent->contains.push_back(this);
		}
	}
}
