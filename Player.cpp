#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleEntities.h"
#include "PlayerStates.h"
#include "Player.h"
#include "JsonHandler.h"

Player::Player(int number_player, SDL_Texture* texture, const char* name, ModuleStages* stage, Entity* parent) : Creature(Entity::Type::PLAYER, texture, stage, parent, true), number_player(number_player)
{
	if (App->parser->LoadObject(name))
	{
		ispeed = App->parser->GetInt("Speed");
		dimensions.z = App->parser->GetInt("Depth");

		moving = new Player_MoveState(this, "Move_Animation", "MoveUp_Animation");
		idle = new Player_StandState(this, "Static_Frame");
		jumping = new Player_JumpState(this, "JumpFrame");

		if (App->parser->UnloadObject() == true)
		{
			current_state = idle;
			entity_rect = idle->initial_rect;
			dimensions.x = idle->initial_rect.w;
			dimensions.y = idle->initial_rect.h;
		}
	}
}

Player::~Player()
{
	RELEASE(moving);
	RELEASE(idle);
	RELEASE(jumping);
}

update_status Player::PreUpdate()
{
	HandleInput();

	return UPDATE_CONTINUE;
}

update_status Player::Update()
{
	update_status ret = UPDATE_CONTINUE;

	current_state->Update();

	last_zmov = zmovement;

	return ret;
}

void Player::HandleInput()
{

	zmovement = ZDirection::YIDLE;
	if (App->input->GetPlayerOutput(number_player, PlayerOutput::GO_UP))
		zmovement = ZDirection::UP;
	else if (App->input->GetPlayerOutput(number_player, PlayerOutput::GO_DOWN))
		zmovement = ZDirection::DOWN;
		
	xmovement = XDirection::XIDLE;
	if (App->input->GetPlayerOutput(number_player, PlayerOutput::GO_LEFT))
		xmovement = XDirection::LEFT;
	else if (App->input->GetPlayerOutput(number_player, PlayerOutput::GO_RIGHT))
		xmovement = XDirection::RIGHT;


	attack_cmd = Attack::NOATTACK;
	if (App->input->GetPlayerOutput(number_player, PlayerOutput::PUNCH))
		attack_cmd = Attack::PUNCH;
	else if (App->input->GetPlayerOutput(number_player, PlayerOutput::KICK))
		attack_cmd = Attack::KICK;
		
	jump = false;
	if (App->input->GetPlayerOutput_KeyDown(number_player, PlayerOutput::JUMP))
		jump = true;

	current_state = current_state->HandleInput();
}