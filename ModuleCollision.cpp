#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFonts.h"
#include "Entity.h"

ModuleCollision::ModuleCollision() : Module(MODULE_COLLISION)
{
}

ModuleCollision::~ModuleCollision()
{
}

update_status ModuleCollision::PreUpdate()
{
	for (std::list<Collider*>::iterator it = creatures.begin(); it != creatures.end();)
	{
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = creatures.erase(it);
		}
		else
			++it;
	}

	for (std::list<Collider*>::iterator it = attacks.begin(); it != attacks.end();)
	{
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = attacks.erase(it);
		}
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleCollision::Update()
{
	for (std::list<Collider*>::iterator it = attacks.begin(); it != attacks.end(); ++it)
	{
		for (std::list<Collider*>::iterator jt = creatures.begin(); jt != creatures.end(); ++jt)
		{
			if (ColliderMatrix[(*it)->type][(*jt)->type])
			{
				if ((*it)->active && (*jt)->active)
				{
					if ((*it)->CheckCollision(**jt))
					{
						(*jt)->parent->HasCollided((*it));
					}
				}
			}
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleCollision::DebugDraw()
{
	for (std::list<Collider*>::iterator it = creatures.begin(); it != creatures.end(); ++it)
	{
		if ((*it)->active)
		{
			SDL_Rect collider_rect = { (*it)->position.x,(*it)->position.y,(*it)->dimensions.x,(*it)->dimensions.y };
			App->renderer->DrawQuad(collider_rect, 255, 0, 0, 80);
		}	
	}

	for (std::list<Collider*>::iterator it = attacks.begin(); it != attacks.end(); ++it)
	{
		if ((*it)->active)
		{
			SDL_Rect collider_rect = { (*it)->position.x,(*it)->position.y,(*it)->dimensions.x,(*it)->dimensions.y };
			App->renderer->DrawQuad(collider_rect, 0, 0, 255, 80);
		}
	}
}

Collider* ModuleCollision::AddCollider(const Point3d& position, const Point3d& dimensions, ColliderType type, Entity* parent)
{
	Collider* ret = new Collider(position, dimensions, type, parent);

	if (type == PLAYER || type == ENEMY)
		creatures.push_back(ret);
	else if (type == PLAYER_ATTACK || type == ENEMY_ATTACK)
		attacks.push_back(ret);

	return ret;
}

bool Collider::CheckCollision(const Collider& collider) const
{
	bool ret = true;

	if (collider.position.x < position.x && collider.position.x + collider.dimensions.x < position.x)
		ret = false;
	else if (position.x < collider.position.x && position.x + dimensions.x < collider.position.x)
		ret = false;
	else if (collider.position.y < position.y && collider.position.y + collider.dimensions.y < position.y)
		ret = false;
	else if (position.y < collider.position.y && position.y + dimensions.y < collider.position.y)
		ret = false;
	else if (collider.position.z < position.z && collider.position.z + collider.dimensions.z < position.z)
		ret = false;
	else if (position.z < collider.position.z && position.z + dimensions.z < collider.position.z)
		ret = false;

	return ret;
}
