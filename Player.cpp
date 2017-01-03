#include "Application.h"
#include "ModuleInput.h"
#include "ModuleEntities.h"
#include "PlayerStates.h"
#include "Player.h"
#include "JsonHandler.h"

Player::Player(int number_player, SDL_Texture* texture, const char* name, Entity* parent) : Entity(Entity::Type::PLAYER, texture, parent), number_player(number_player)
{
	if (App->parser->LoadObject(name))
	{
		ispeed = App->parser->GetInt("Speed");

		moving = new Player_MoveState(this, "Move_Animation", "MoveUp_Animation");
		idle = new Player_StandState(this, "Static_Frame");

		depth = App->parser->GetInt("Depth");

		if (App->parser->UnloadObject() == true)
		{
			current_state = idle;
			width = idle->initial_rect.w;
			height = idle->initial_rect.h;
		}
	}
	texture = App->entities->players;
}

Player::~Player()
{
	RELEASE(moving);
	RELEASE(idle);
}

update_status Player::PreUpdate()
{
	update_status ret = UPDATE_CONTINUE;

	if (App->input->GetPlayerOutput(number_player, PlayerOutput::GO_UP))
		zmovement = ZDirection::UP;
	else if (App->input->GetPlayerOutput(number_player, PlayerOutput::GO_DOWN))
		zmovement = ZDirection::DOWN;
	else
		zmovement = ZDirection::YIDLE;

	if (App->input->GetPlayerOutput(number_player, PlayerOutput::GO_LEFT))
		xmovement = XDirection::LEFT;
	else if (App->input->GetPlayerOutput(number_player, PlayerOutput::GO_RIGHT))
		xmovement = XDirection::RIGHT;
	else
		xmovement = XDirection::XIDLE;

	if (App->input->GetPlayerOutput(number_player, PlayerOutput::PUNCH))
		attack_cmd = Attack::PUNCH;
	else if (App->input->GetPlayerOutput(number_player, PlayerOutput::KICK))
		attack_cmd = Attack::KICK;
	else
		attack_cmd = Attack::NOATTACK;

	if (App->input->GetPlayerOutput(number_player, PlayerOutput::JUMP))
		jump = true;
	else
		jump = false;

	current_state = current_state->HandleInput();

	return ret;
}

update_status Player::Update()
{
	update_status ret = UPDATE_CONTINUE;

	current_state->Update();

	last_zmov = zmovement;

	return ret;
}