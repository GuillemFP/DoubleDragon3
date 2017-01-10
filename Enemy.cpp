#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleEntities.h"
#include "ModuleCollision.h"
#include "Enemy.h"
#include "EnemyStates.h"
#include "Player.h"
#include "JsonHandler.h"
#include "Timer.h"

Enemy::Enemy(const char* name, ModuleStages* stage, Entity* parent) : Creature(Entity::Type::ENEMY, App->entities->enemies, stage, parent, true)
{
	logic_timer = new Timer();

	inverted_texture = true;

	if (App->parser->LoadObject(name))
	{
		ispeed = App->parser->GetInt("Speed");
		dimensions.z = App->parser->GetInt("Depth");
		health = abs(App->parser->GetInt("Health"));
		float immunity_seconds = App->parser->GetFloat("ImmunityAfterAttack");
		damage_treshold = App->parser->GetInt("MaxAccumulatedDamage");
		
		float logic_time = App->parser->GetFloat("LogicTime");
		attack_range = App->parser->GetInt("AttackRange");
		sight_range = App->parser->GetInt("SightRange");
		
		blink_ratio = App->parser->GetFloat("BlinkRatio");

		sound_jump = App->entities->GetSound(App->parser->GetInt("Sound_Jump"));
		sound_dying = App->entities->GetSound(App->parser->GetInt("Sound_Dying"));

		moving = new Enemy_MoveState(this, "Move_Animation", "MoveUp_Animation");
		idle = new Enemy_StandState(this, "Static_Frame");
		jumping = new Enemy_JumpState(this, "JumpFrame", "AerialKickFrame");
		attacking = new Enemy_AttackState(this, "Attack_Animation", nullptr);
		damaging = new Enemy_DamageState(this, "DamageHigh_Frame", "DamageLow_Frame");
		falling = new Enemy_FallState(this, "Falling_Frame", "Fallen_Frame", "Rising_Frame");

		current_state = idle;

		if (App->parser->UnloadObject() == true)
		{
			current_state = idle;
			entity_rect = idle->initial_rect;
			dimensions.x = idle->initial_rect.w;
			dimensions.y = idle->initial_rect.h;

			immunity_after_attack->SetMaxTime((Uint32)(immunity_seconds * 1000.0f));

			logic_timer->SetMaxTime((Uint32)(logic_time*1000.0f));
			blink->SetMaxTime(((Uint32)(blink_ratio*1000.0f)));
		}

		collider = App->collision->AddCollider(position, dimensions, ColliderType::ENEMY, this);
		attack_collider = App->collision->AddCollider({ 0,0,0 }, { 0,0,0 }, ColliderType::ENEMY_ATTACK, this);
		attack_collider->active = false;

		logic_timer->Start();
	}
	immunity_after_attack->Start();
}

Enemy::~Enemy()
{
}

update_status Enemy::PreUpdate()
{
	current_state = current_state->Logic();

	return UPDATE_CONTINUE;
}

update_status Enemy::Update()
{
	current_state->Update();

	return UPDATE_CONTINUE;
}

void Enemy::HasCollided(Collider * with)
{
	if (immunity_after_attack->MaxTimeReached())
	{
		bool fall = false;
		if (current_state == jumping)
			fall = true;

		accumulated_damage += with->damage;
		if (accumulated_damage >= damage_treshold)
		{
			accumulated_damage -= damage_treshold;
			accumulated_damage = 0;
			fall = true;
		}
		health -= with->damage;
		if (health < 0)
		{
			health = 0;
			accumulated_damage = 0;
			fall = true;
		}

		if (fall == false)
		{
			collider->active = false;
			current_state->OnExit();
			current_state = damaging;
			damaging->in_damaged_state->Start();
			damaging->SetFrame(with->attack_type);
		}
		else
		{
			collider->active = false;
			if (health == 0)
				App->audio->PlayFx(sound_dying);

			if (position.x < with->parent->position.x)
			{
				inverted_texture = false;
				falling->InitFall(Creature::XDirection::LEFT);
			}
			else
			{
				inverted_texture = true;
				falling->InitFall(Creature::XDirection::RIGHT);
			}

			current_state->OnExit();
			current_state = falling;
		}

		immunity_after_attack->Reset();
	}
}

bool Enemy::TargetWithinAttackRange() const
{
	bool ret = true;

	switch (target_xdirection)
	{
	case Creature::XDirection::LEFT:
		if (position.x - current_target->position.x > current_target->dimensions.x + attack_range)
			ret = false;
		break;
	case Creature::XDirection::RIGHT:
		if (current_target->position.x - position.x > dimensions.x + attack_range)
			ret = false;
		break;
	}

	switch (target_zdirection)
	{
	case Creature::ZDirection::UP:
		if (position.z - current_target->position.z > dimensions.z + current_target->dimensions.z)
			ret = false;
		break;
	case Creature::ZDirection::DOWN:
		if (current_target->position.z - position.z > dimensions.z + current_target->dimensions.z)
			ret = false;
		break;
	}

	return ret;
}

bool Enemy::TargetWithinSightRange() const
{
	if (abs(current_target->position.x - position.x) < sight_range)
		return true;
	else
		return false;
}

void Enemy::SetDestination()
{
	final_destination.x = current_target->position.x;
	switch (target_xdirection)
	{
	case Creature::LEFT:
		if (position.x - final_destination.x > current_target->dimensions.x + attack_range)
			final_destination.x += current_target->dimensions.x + attack_range;
		break;
	case Creature::XIDLE:
		break;
	case Creature::RIGHT:
		if (final_destination.x - position.x > dimensions.x + attack_range)
			final_destination.x -= dimensions.x + attack_range;
		break;
	}

	final_destination.y = current_target->position.z;
	switch (target_zdirection)
	{
	case Creature::UP:
		if (position.z - final_destination.y > current_target->dimensions.z)
			final_destination.y += current_target->dimensions.z;
		break;
	case Creature::YIDLE:
		break;
	case Creature::DOWN:
		if (final_destination.y - position.z > dimensions.z)
			final_destination.y -= dimensions.z;
		break;
	}
}

bool Enemy::DestinationReached() const
{
	bool ret = true;

	if (final_destination.x != position.x)
		ret = false;
	else if (final_destination.y != position.z)
		ret = false;

	return ret;
}
