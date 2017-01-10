#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleEntities.h"
#include "ModuleCollision.h"
#include "PlayerStates.h"
#include "Player.h"
#include "JsonHandler.h"

Player::Player(int number_player, const char* name, ModuleStages* stage, Entity* parent) : Creature(Entity::Type::PLAYER, App->entities->players, stage, parent, true), number_player(number_player)
{
	if (App->parser->LoadObject(name))
	{
		this->name = App->parser->GetString("Name");
		ispeed = App->parser->GetInt("Speed");
		dimensions.z = App->parser->GetInt("Depth");
		initial_health = abs(App->parser->GetInt("Health"));
		float immunity_seconds = App->parser->GetFloat("ImmunityAfterAttack");
		App->parser->GetRect(face, "Face");
		damage_treshold = App->parser->GetInt("MaxAccumulatedDamage");

		blink_time_alive = App->parser->GetFloat("BlinkTimeAlive");
		blink_ratio = App->parser->GetFloat("BlinkRatio");

		sound_attack = App->entities->GetSound(App->parser->GetInt("Sound_Attack"));
		sound_jump = App->entities->GetSound(App->parser->GetInt("Sound_Jump"));
		sound_hit = App->entities->GetSound(App->parser->GetInt("Sound_Damaged"));
		sound_falling = App->entities->GetSound(App->parser->GetInt("Sound_Falling"));
		sound_dying = App->entities->GetSound(App->parser->GetInt("Sound_Dying"));

		moving = new Player_MoveState(this, "Move_Animation", "MoveUp_Animation");
		idle = new Player_StandState(this, "Static_Frame");
		jumping = new Player_JumpState(this, "JumpFrame", "AerialKickFrame");
		attacking = new Player_AttackState(this, "Punch_Animation", "Kick_Animation");
		damaging = new Player_DamageState(this, "DamageHigh_Frame", "DamageLow_Frame");
		falling = new Player_FallState(this, "Falling_Frame", "Fallen_Frame", "Rising_Frame");

		current_state = idle;

		if (App->parser->UnloadObject() == true)
		{
			health = initial_health;

			current_state = idle;
			entity_rect = idle->initial_rect;
			dimensions.x = idle->initial_rect.w;
			dimensions.y = idle->initial_rect.h;

			immunity_after_attack->SetMaxTime((Uint32)(immunity_seconds * 1000.0f));
			blink->SetMaxTime(((Uint32)(blink_ratio*1000.0f)));
		}

		collider = App->collision->AddCollider(position, dimensions, ColliderType::PLAYER, this);
		attack_collider = App->collision->AddCollider({ 0,0,0 }, { 0,0,0 }, ColliderType::PLAYER_ATTACK, this);
		attack_collider->active = false;
	}

	reviving_timer = new Timer((Uint32)(blink_time_alive*1000.0f));
	immunity_after_attack->Start();
}

Player::~Player()
{
	RELEASE(moving);
	RELEASE(idle);
	RELEASE(jumping);
	RELEASE(attacking);
	RELEASE(damaging);
	RELEASE(falling);
}

void Player::Revive()
{
	reviving = true;
	active = true;
	collider->active = false;
	blinking = true;
	health = initial_health;
	accumulated_damage = 0;
	reviving_timer->Start();
	blink->Reset();
	current_state = idle;
	idle->current_rect = idle->initial_rect;
}

void Player::Rising()
{
	collider->active = false;
	rising = true;
	blinking = true;
	accumulated_damage = 0;
	reviving_timer->Start();
	blink->Reset();
}

update_status Player::PreUpdate()
{
	jump = false;
	if (App->input->GetPlayerOutput_KeyDown(number_player, PlayerOutput::JUMP))
		jump = true;

	xmovement = XDirection::XIDLE;
	if (App->input->GetPlayerOutput(number_player, PlayerOutput::GO_LEFT))
		xmovement = XDirection::LEFT;
	else if (App->input->GetPlayerOutput(number_player, PlayerOutput::GO_RIGHT))
		xmovement = XDirection::RIGHT;

	zmovement = ZDirection::YIDLE;
	if (App->input->GetPlayerOutput(number_player, PlayerOutput::GO_UP))
		zmovement = ZDirection::UP;
	else if (App->input->GetPlayerOutput(number_player, PlayerOutput::GO_DOWN))
		zmovement = ZDirection::DOWN;

	attack_cmd = Attack::NOATTACK;
	if (App->input->GetPlayerOutput_KeyDown(number_player, PlayerOutput::PUNCH))
		attack_cmd = Attack::PUNCH;
	else if (App->input->GetPlayerOutput_KeyDown(number_player, PlayerOutput::KICK))
		attack_cmd = Attack::KICK;

	current_state = current_state->HandleInput();

	return UPDATE_CONTINUE;
}

update_status Player::Update()
{
	update_status ret = UPDATE_CONTINUE;

	current_state->Update();

	if (reviving && reviving_timer->MaxTimeReached())
	{
		reviving_timer->Stop();
		reviving = false;
		dead = false;
		collider->active = true;
		draw = true;
		blinking = false;
	}

	if (rising && reviving_timer->MaxTimeReached())
	{
		reviving_timer->Stop();
		rising = false;
		collider->active = true;
		draw = true;
		blinking = false;
	}

	return ret;
}

void Player::HasCollided(Collider* with)
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
			App->audio->PlayFx(sound_hit);
			collider->active = false;
			current_state->OnExit();
			current_state = damaging;
			damaging->in_damaged_state->Start();
			damaging->SetFrame(with->attack_type);
		}
		else
		{
			App->audio->PlayFx(sound_falling);
			if (health == 0)
				App->audio->PlayFx(sound_dying);
			collider->active = false;

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