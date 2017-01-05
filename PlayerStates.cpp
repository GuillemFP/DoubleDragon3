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

	if (player->zmovement == Player::ZDirection::UP)
	{
		ret = player->moving;
		player->moving->current_animation = &player->moving->moving_up;
	}
	else if (player->zmovement != Player::ZDirection::YIDLE || player->xmovement != Player::XDirection::XIDLE)
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

	if (player->zmovement == Player::ZDirection::UP)
	{
		ret = this;
		current_animation = &moving_up;
	}
	else if (player->zmovement != Player::ZDirection::YIDLE || player->xmovement != Player::XDirection::XIDLE)
	{
		ret = this;
		current_animation = &moving;
	}
	else
	{
		if (player->last_zmov == Player::ZDirection::UP)
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
	player->x += player->xmovement*player->ispeed;
	player->y += player->zmovement*player->ispeed;
	player->z += player->zmovement*player->ispeed;

	if (player->xmovement == Player::XDirection::LEFT)
		player->inverted_texture = true;
	else if (player->xmovement == Player::XDirection::RIGHT)
		player->inverted_texture = false;

	player->entity_rect = current_animation->GetCurrentFrame();

	return UPDATE_CONTINUE;
}