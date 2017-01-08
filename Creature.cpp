#include "Creature.h"
#include "ModuleCollision.h"

Creature::Creature(Entity::Type type, SDL_Texture* texture, ModuleStages* stage, Entity* parent, bool active) : Entity(Entity::Type::PLAYER, texture, stage, parent, true) 
{
	immunity_after_attack = new Timer();
}

Creature::~Creature()
{
	RELEASE(immunity_after_attack);
}

void Creature::HasCollided(Collider* with)
{
	if (immunity_after_attack->MaxTimeReached())
	{
		health -= with->damage;
		if (health < 0)
			health = 0;
		immunity_after_attack->Reset();
	}
}