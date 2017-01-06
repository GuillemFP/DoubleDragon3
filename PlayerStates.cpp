#include "Application.h"
#include "ModuleRender.h"
#include "JsonHandler.h"
#include "SDL/include/SDL_scancode.h"
#include "Player.h"
#include "PlayerStates.h"

Player_StandState::Player_StandState(Player* player, const char* staticframe) : PlayerState(player)
{
	App->parser->GetRect(initial_rect, staticframe);
}

PlayerState* Player_StandState::HandleInput()
{
	PlayerState* ret = this;

	if (player->jump == true)
	{
		player->jumping->SetJumpParameters(player->position.y, player->xmovement);
		ret = player->jumping;
	}
	else if (player->zmovement == Creature::ZDirection::UP)
	{
		ret = player->moving;
		player->moving->current_animation = &player->moving->moving_up;
	}
	else if (player->zmovement != Creature::ZDirection::YIDLE || player->xmovement != Creature::XDirection::XIDLE)
	{
		ret = player->moving;
		player->moving->current_animation = &player->moving->moving;
	}

	return ret;
}

Player_MoveState::Player_MoveState(Player* player, const char* move_animation, const char* moveup_animation) : PlayerState(player)
{
	App->parser->GetAnimation(moving, move_animation);
	App->parser->GetAnimation(moving_up, moveup_animation);
	current_animation = &moving;
}

PlayerState* Player_MoveState::HandleInput()
{
	PlayerState* ret = player->idle;

	if (player->jump == true)
	{
		player->jumping->SetJumpParameters(player->position.y, player->xmovement);
		ret = player->jumping;
	}
	else if (player->zmovement == Creature::ZDirection::UP)
	{
		ret = this;
		current_animation = &moving_up;
	}
	else if (player->zmovement != Creature::ZDirection::YIDLE || player->xmovement != Creature::XDirection::XIDLE)
	{
		ret = this;
		current_animation = &moving;
	}
	else
	{
		if (player->last_zmov == Creature::ZDirection::UP)
		{
			player->entity_rect = player->idle->initial_rect;
			moving.Reset();
			moving_up.Reset();
		}
	}

	return ret;
}

update_status Player_MoveState::Update()
{
	bool player_static = true;
	if (player->xmovement == Creature::XDirection::LEFT)
	{
		player->inverted_texture = true;
		if (player->stage->InsideScene_LeftBorder(player->position, player->dimensions))
		{
			player->position.x -= player->ispeed;
			player_static = false;
		}
	}
	else if (player->xmovement == Creature::XDirection::RIGHT)
	{
		player->inverted_texture = false;
		if (player->stage->InsideScene_RightBorder(player->position, player->dimensions))
		{
			player->position.x += player->ispeed;
			player_static = false;
		}
	}

	if (player->zmovement == Creature::ZDirection::UP)
	{
		if (player->stage->InsideScene_HighBorder(player->position, player->dimensions))
		{
			player->position.z -= player->ispeed;
			player->position.y -= player->ispeed;
			player_static = false;
		}
	}
	else if (player->zmovement == Creature::ZDirection::DOWN)
	{
		if (player->stage->InsideScene_LowBorder(player->position, player->dimensions))
		{
			player->position.z += player->ispeed;
			player->position.y += player->ispeed;
			player_static = false;
		}
	}

	if (player_static == false)
		player->entity_rect = current_animation->GetCurrentFrame();

	return UPDATE_CONTINUE;
}

Player_JumpState::Player_JumpState(Player* player, const char* jumpframe) : PlayerState(player)
{
	App->parser->GetRect(jumprect, jumpframe);
	speed_y = App->parser->GetInt("JumpSpeedY");
	speed_x = App->parser->GetInt("JumpSpeedX");
	gravity = App->parser->GetFloat("Gravity");
}

PlayerState* Player_JumpState::HandleInput()
{
	return this;
}

update_status Player_JumpState::Update()
{
	player->position.y += (int)(gravity * time) - speed_y;

	if (jump_direction == Creature::XDirection::LEFT)
		if (player->stage->InsideScene_LeftBorder(player->position, player->dimensions))
			player->position.x -= speed_x;

	if (jump_direction == Creature::XDirection::RIGHT)
		if (player->stage->InsideScene_RightBorder(player->position, player->dimensions))
			player->position.x += speed_x;

	time++;

	if (initial_y == player->position.y)
	{
		player->current_state = player->idle;
		player->entity_rect = player->idle->initial_rect;
	}
	else
		player->entity_rect = jumprect;

	return UPDATE_CONTINUE;
}

void Player_JumpState::SetJumpParameters(int initial_y, Creature::XDirection jump_direction)
{
	time = 0;
	this->initial_y = initial_y;
	this->jump_direction = jump_direction;
}