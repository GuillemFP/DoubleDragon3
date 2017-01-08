#include "Application.h"
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
		health = abs(App->parser->GetInt("Health"));
		float immunity_seconds = App->parser->GetFloat("ImmunityAfterAttack");
		App->parser->GetRect(face, "Face");

		sound_attack = App->entities->GetSound(App->parser->GetInt("Sound_Attack"));
		sound_jump = App->entities->GetSound(App->parser->GetInt("Sound_Jump"));

		moving = new Player_MoveState(this, "Move_Animation", "MoveUp_Animation");
		idle = new Player_StandState(this, "Static_Frame");
		jumping = new Player_JumpState(this, "JumpFrame", "AerialKickFrame");
		attacking = new Player_AttackState(this, "Punch_Animation", "Kick_Animation");
		damaging = new Player_DamageState(this, "DamageHigh_Frame", "DamageLow_Frame");

		current_state = idle;

		if (App->parser->UnloadObject() == true)
		{
			current_state = idle;
			entity_rect = idle->initial_rect;
			dimensions.x = idle->initial_rect.w;
			dimensions.y = idle->initial_rect.h;

			immunity_after_attack->SetMaxTime((Uint32)(immunity_seconds * 1000.0f));
		}

		//collider = App->collision->AddCollider(&position, &dimensions, ColliderType::PLAYER, this);
		collider = App->collision->AddCollider(&collider_position, &collider_dimensions, ColliderType::PLAYER, this);
		attack_collider = App->collision->AddCollider(&attack_position, &attack_dimensions, ColliderType::PLAYER_ATTACK, this);
		attack_collider->active = false;
	}

	immunity_after_attack->Start();
}

Player::~Player()
{
	RELEASE(moving);
	RELEASE(idle);
	RELEASE(jumping);
	RELEASE(attacking);
	RELEASE(damaging);
}

update_status Player::PreUpdate()
{
	HandleInput();

	return UPDATE_CONTINUE;
}

update_status Player::Update()
{
	update_status ret = UPDATE_CONTINUE;

	collider_position = position;
	collider_dimensions = dimensions;

	current_state->Update();

	return ret;
}

void Player::HasCollided(Collider* with)
{
	if (immunity_after_attack->MaxTimeReached())
	{
		
		if (current_state != jumping)
		{
			collider->active = false;
			current_state->OnExit();
			current_state = damaging;
			damaging->SetFrame(with->attack_type);
		}
	}
	Creature::HasCollided(with);
}

void Player::HandleInput()
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
}