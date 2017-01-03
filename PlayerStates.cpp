#include "Application.h"
#include "ModuleRender.h"
#include "JsonHandler.h"
#include "SDL/include/SDL_scancode.h"
#include "Player.h"
#include "PlayerStates.h"

Player_StandState::Player_StandState(Player* player, const char* parser_name) : PlayerState(player)
{
	App->parser->GetRect(initial_rect, parser_name);
	current_rect = initial_rect;
}

Player_StandState::~Player_StandState()
{
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

update_status Player_StandState::Update()
{
	App->renderer->Blit(player->texture, { player->x, player->y }, &current_rect, player->inverted);

	return UPDATE_CONTINUE;
}

Player_MoveState::Player_MoveState(Player* parent, const char* move_animation, const char* moveup_animation) : PlayerState(parent)
{
	App->parser->GetAnimation(moving, move_animation);
	App->parser->GetAnimation(moving_up, moveup_animation);
	current_animation = &moving;
}

Player_MoveState::~Player_MoveState()
{
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
			player->idle->current_rect = player->idle->initial_rect;
			moving.Reset();
			moving_up.Reset();
		}
		else
			player->idle->current_rect = current_rect;
	}

	return ret;
}

update_status Player_MoveState::Update()
{
	player->x += player->xmovement*player->ispeed;
	player->y -= player->zmovement*player->ispeed;
	player->z += player->zmovement*player->ispeed;

	if (player->xmovement == Player::XDirection::LEFT)
		player->inverted = true;
	else if (player->xmovement == Player::XDirection::RIGHT)
		player->inverted = false;

	current_rect = current_animation->GetCurrentFrame();

	App->renderer->Blit(player->texture, { player->x, player->y }, &current_rect, player->inverted);

	return UPDATE_CONTINUE;
}