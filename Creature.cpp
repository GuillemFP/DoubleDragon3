#include "Creature.h"
#include "ModuleCollision.h"

Creature::Creature(Entity::Type type, SDL_Texture* texture, ModuleStages* stage, Entity* parent, bool active) : Entity(Entity::Type::PLAYER, texture, stage, parent, true) 
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
	if (dead == false || blinking_on == false)
		Entity::Draw();

	if (dead == true)
	{
		blink->Start((Uint32)(blink_ratio*1000.0f));
		if (blink->MaxTimeReached())
		{
			blink->Reset();
			blinking_on = !blinking_on;
		}
	}

	return active;
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