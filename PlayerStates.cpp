#include "Application.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "JsonHandler.h"
#include "SDL/include/SDL_scancode.h"
#include "Player.h"
#include "PlayerStates.h"

Player_StandState::Player_StandState(Player* player, const char* staticframe) : PlayerState(player)
{
	App->parser->GetRect(initial_rect, staticframe);
	current_rect = initial_rect;
}

PlayerState* Player_StandState::HandleInput()
{
	PlayerState* ret = this;

	ret = player->moving->HandleInput();

	return ret;
}

update_status Player_StandState::Update() 
{ 
	player->entity_rect = current_rect;
	return UPDATE_CONTINUE; 
}

void Player_StandState::OnExit()
{
	current_rect = initial_rect;
}

Player_MoveState::Player_MoveState(Player* player, const char* move_animation, const char* moveup_animation) : PlayerState(player)
{
	App->parser->GetAnimation(moving, move_animation);
	App->parser->GetAnimation(moving_up, moveup_animation);
	App->parser->GetRect(last_moving, "Static_Frame");
	current_animation = &moving;
}

PlayerState* Player_MoveState::HandleInput()
{
	PlayerState* ret = this;

	player->idle->current_rect = last_moving;

	if (player->jump == true)
		ret = player->jumping->HandleInput();
	else
	{
		switch (player->attack_cmd)
		{
		case Creature::Attack::NOATTACK:
		{
			current_animation = &moving;
			switch (player->xmovement)
			{
			case Creature::XDirection::LEFT:
				switch (player->zmovement)
				{
				case Creature::ZDirection::UP:
					last_moving = player->idle->initial_rect;
					current_animation = &moving_up;
					moving.Reset();
				case Creature::ZDirection::DOWN:
					if (player->in_plataform && player->stage->InPlataform(player->position.x + player->dimensions.x, player->position.z) == false)
						ret = player->jumping->HandleInput();
					break;
				case Creature::ZDirection::YIDLE:
					if (player->in_plataform && player->stage->InPlataform(player->position.x, player->position.z) == false)
						ret = player->jumping->HandleInput();
					moving_up.Reset();
					break;
				}
				break;
			case Creature::XDirection::RIGHT:
				switch (player->zmovement)
				{
				case Creature::ZDirection::UP:
					last_moving = player->idle->initial_rect;
					current_animation = &moving_up;
					moving.Reset();
					break;
				case Creature::ZDirection::DOWN:
					if (player->in_plataform && player->stage->InPlataform(player->position.x + player->dimensions.x, player->position.z) == false)
						ret = player->jumping->HandleInput();
					break;
				case Creature::ZDirection::YIDLE:
					moving_up.Reset();
					break;
				}
				break;
			case Creature::XDirection::XIDLE:
				switch (player->zmovement)
				{
				case Creature::ZDirection::UP:
					last_moving = player->idle->initial_rect;
					current_animation = &moving_up;
					moving.Reset();
					break;
				case Creature::ZDirection::DOWN:
					if (player->in_plataform && player->stage->InPlataform(player->position.x + player->dimensions.x, player->position.z) == false)
						ret = player->jumping->HandleInput();
					break;
				case Creature::ZDirection::YIDLE:
					ret = player->idle;
					player->idle->current_rect = last_moving;
					current_animation = &moving;
					moving_up.Reset();
					break;
				default:
					break;
				}
			}
		}
		break;
		case Creature::Attack::PUNCH:
		case Creature::Attack::KICK:
			ret = player->attacking->HandleInput();
			break;
		}
	}

	if (ret != this && ret != player->idle)
	{
		/*moving.Reset();
		last_moving = player->idle->initial_rect;
		player->idle->current_rect = last_moving;*/
		OnExit();
	}

	return ret;
}

update_status Player_MoveState::Update()
{
	bool player_static = true;

	switch (player->xmovement)
	{
	case Creature::XDirection::LEFT:
		player->inverted_texture = true;
		if (player->stage->InsideScene_LeftBorder(player->position, player->dimensions, player->in_plataform))
		{
			player->position.x -= player->ispeed;
			player_static = false;
		}
		break;
	case Creature::XDirection::RIGHT:
		player->inverted_texture = false;
		if (player->stage->InsideScene_RightBorder(player->position, player->dimensions, player->in_plataform))
		{
			player->position.x += player->ispeed;
			player_static = false;
		}
	}

	switch (player->zmovement)
	{
	case Creature::ZDirection::UP:
		if (player->stage->InsideScene_HighBorder(player->position, player->dimensions, player->in_plataform))
		{
			player->position.z -= player->ispeed;
			player->position.y -= player->ispeed;
			player_static = false;
		}
		break;
	case Creature::ZDirection::DOWN:
		if (player->stage->InsideScene_LowBorder(player->position, player->dimensions))
		{
			player->position.z += player->ispeed;
			player->position.y += player->ispeed;
			player_static = false;
		}
		break;
	}

	if (current_animation == &moving)
	{
		if (player_static == false)
			last_moving = current_animation->GetCurrentFrame();
		player->entity_rect = last_moving;
	}
	else if (current_animation == &moving_up)
	{
		if (player_static == false)
			last_moving_up = current_animation->GetCurrentFrame();
		player->entity_rect = last_moving_up;
	}		

	return UPDATE_CONTINUE;
}

void Player_MoveState::OnExit()
{
	moving.Reset();
	moving_up.Reset();
	last_moving = player->idle->initial_rect;
	player->idle->current_rect = last_moving;
}

Player_JumpState::Player_JumpState(Player* player, const char* jump_frame, const char* jumpattack_frame) : PlayerState(player)
{
	App->parser->GetRect(jump_rect, jump_frame);
	App->parser->GetRect(jumpkick_rect, jumpattack_frame);
	App->parser->GetPoint(speed, "JumpSpeed");
	gravity = App->parser->GetFloat("Gravity");

	App->parser->GetRect(kick_rect, "AerialKickRect");
	kick_damage = App->parser->GetInt("AerialKickDamage");
}

PlayerState* Player_JumpState::HandleInput()
{
	if (time == 0)
		SetJumpParameters();
	else if (attacking == false && player->attack_cmd == Creature::Attack::KICK)
	{
		App->audio->PlayFx(player->sound_attack);
		attacking = true;
		player->entity_rect = jumpkick_rect;
		player->attack_collider->damage = kick_damage;

		player->attack_position.z = player->position.z;
		player->attack_dimensions.x = kick_rect.w;
		player->attack_dimensions.y = kick_rect.h;
		player->attack_dimensions.z = player->dimensions.z;

		player->attack_collider->active = true;
	}

	return this;
}

update_status Player_JumpState::Update()
{
	current_position.y = (float)(initial_pos.y) - speed.y*time + gravity*time*time;
	if (player->position.y < ((int)(current_position.y + 0.5f)))
		maximum_reached = true;

	player->position.y = (int)(current_position.y + 0.5f);

	switch (jump_direction)
	{
	case Creature::XDirection::LEFT:
		player->inverted_texture = true;
		if (player->stage->InsideScene_LeftBorder(player->position, player->dimensions))
		{
			current_position.x += jump_direction*speed.x;
			player->position.x = (int)(current_position.x + 0.5f);
		}
		break;
	case Creature::XDirection::RIGHT:
		player->inverted_texture = false;
		if (player->stage->InsideScene_RightBorder(player->position, player->dimensions, player->in_plataform))
		{
			current_position.x += jump_direction*speed.x;
			player->position.x = (int)(current_position.x + 0.5f);
		}
		else if (direct_jump == true)
		{
			current_position.x += jump_direction*speed.x;
			player->position.x = (int)(current_position.x + 0.5f);
		}
		else if (jumping_to_platform == true && initial_pos.y - player->position.y > player->stage->plataform_height)
		{
			current_position.x += jump_direction*speed.x;
			player->position.x = (int)(current_position.x + 0.5f);
		}
		break;
	}

	if (player->inverted_texture == false)
		player->attack_position.x = player->position.x + kick_rect.x;
	if (player->inverted_texture == true)
		player->attack_position.x = player->position.x + player->dimensions.x - kick_rect.x - kick_rect.w;
	player->attack_position.y = player->position.y + kick_rect.y;

	time++;

	if (player->position.y >= final_pos.y && maximum_reached == true)
	{
		time = 0;
		player->position.y = final_pos.y;
		player->current_state = player->idle;
		if (jumping_to_platform)
		{
			player->in_plataform = true;
		}
		else if (jumping_off_platform)
		{
			player->in_plataform = false;
		}
		if (attacking == false)
			App->audio->PlayFx(player->sound_jump);
		else
		{
			attacking = false;
			player->attack_collider->active = false;
		}
	}

	return UPDATE_CONTINUE;
}

void Player_JumpState::OnExit()
{
	if (attacking == true)
	{
		attacking = false;
		player->attack_collider->active = false;
	}
}

void Player_JumpState::SetJumpParameters()
{
	time = 1;
	initial_pos.x = player->position.x;
	initial_pos.y = player->position.y;
	final_pos.x = initial_pos.x;
	final_pos.y = initial_pos.y;
	current_position.x = (float)(initial_pos.x);
	current_position.y = (float)(initial_pos.y);

	jump_direction = player->xmovement;
	direct_jump = false;
	jumping_to_platform = false;
	jumping_off_platform = false;
	maximum_reached = false;
	attacking = false;

	final_pos.x += (int)(this->player->xmovement*speed.x*speed.y / gravity);

	switch (player->xmovement)
	{
	case Creature::XDirection::RIGHT:
		if (player->stage->InPlataform(final_pos.x + player->dimensions.x / 2, player->position.z) == true && 
			player->in_plataform == false)
		{
			final_pos.y -= player->stage->plataform_height;
			jumping_to_platform = true;
		}
		else if (player->zmovement == Creature::ZDirection::UP && 
			player->stage->InPlataform(final_pos.x + player->dimensions.x / 2, player->position.z - 1) == true &&
			player->in_plataform == false)
		{
			player->position.z -= 1;
			final_pos.y -= player->stage->plataform_height + 1;
			jumping_to_platform = true;
			direct_jump = true;
		}
		else if (player->zmovement == Creature::ZDirection::DOWN &&
			player->stage->InPlataform(final_pos.x + player->dimensions.x / 2, player->position.z) == false &&
			player->in_plataform == true)
		{
			player->position.z += 1;
			final_pos.y += player->stage->plataform_height + 1;
			jumping_off_platform = true;
		}
		break;
	case Creature::XDirection::LEFT:
		if (player->stage->InPlataform(final_pos.x, player->position.z) == false && player->in_plataform == true)
		{
			final_pos.y += player->stage->plataform_height;
			jumping_off_platform = true;
		}
		else if (player->zmovement == Creature::ZDirection::UP &&
			player->stage->InPlataform(final_pos.x + player->dimensions.x / 2, player->position.z - 1) == true &&
			player->in_plataform == false)
		{
			player->position.z -= 1;
			final_pos.y -= player->stage->plataform_height + 1;
			jumping_to_platform = true;
			direct_jump = true;
		}
		else if (player->zmovement == Creature::ZDirection::DOWN &&
			player->stage->InPlataform(final_pos.x + player->dimensions.x / 2, player->position.z) == false &&
			player->in_plataform == true)
		{
			player->position.z += 1;
			final_pos.y += player->stage->plataform_height + 1;
			jumping_off_platform = true;
		}
		break;
	case Creature::XDirection::XIDLE:
		if (player->zmovement == Creature::ZDirection::DOWN && 
			player->stage->InPlataform(final_pos.x, player->position.z) == false &&
			player->in_plataform == true)
		{
			player->position.z += 1;
			final_pos.y += player->stage->plataform_height + 1;
			jumping_off_platform = true;
		}
		if (player->zmovement == Creature::ZDirection::UP &&
			player->stage->InPlataform(final_pos.x + player->dimensions.x, player->position.z - 1) == true &&
			player->in_plataform == false)
		{
			player->position.z -= 1;
			final_pos.y -= player->stage->plataform_height + 1;
			jumping_to_platform = true;
		}
	}

	player->entity_rect = jump_rect;
}

Player_AttackState::Player_AttackState(Player* player, const char* punch_animation, const char* kick_animation) : PlayerState(player)
{
	App->parser->GetAnimation(punch, punch_animation);
	App->parser->GetAnimation(kick, kick_animation);

	punch_damage = App->parser->GetInt("PunchDamage");
	punch_frame = App->parser->GetInt("PunchDamageFrame");
	App->parser->GetRect(punch_rect, "PunchRect");
	
	kick_damage = App->parser->GetInt("KickDamage");
	kick_frame = App->parser->GetInt("KickDamageFrame");
	App->parser->GetRect(kick_rect, "KickRect");

	current_animation = &punch;
}

PlayerState* Player_AttackState::HandleInput()
{
	if (attacking == false)
	{
		attacking = true;
		App->audio->PlayFx(player->sound_attack);
		player->attack_collider->attack_type = player->attack_cmd;
		switch (player->attack_cmd)
		{
		case Creature::Attack::PUNCH:
			current_animation = &punch;
			player->attack_collider->damage = punch_damage;
			attack_frame = punch_frame;
			attack_rect = &punch_rect;
			break;
		case Creature::Attack::KICK:
			current_animation = &kick;
			player->attack_collider->damage = kick_damage;
			attack_frame = kick_frame;
			attack_rect = &kick_rect;
			break;
		default:
			break;
		}
		if (player->inverted_texture == false)
			player->attack_position.x = player->position.x + attack_rect->x;
		if (player->inverted_texture == true)
			player->attack_position.x = player->position.x + player->dimensions.x - attack_rect->x - attack_rect->w;
		player->attack_position.y = player->position.y + attack_rect->y;
		player->attack_position.z = player->position.z;
		player->attack_dimensions.x = attack_rect->w;
		player->attack_dimensions.y = attack_rect->h;
		player->attack_dimensions.z = player->dimensions.z;
	}

	return this;
}

update_status Player_AttackState::Update()
{
	player->entity_rect = current_animation->GetCurrentFrame();
	if (current_animation->GetCurrentFrameNumber() == attack_frame && player->attack_collider->active == false)
		player->attack_collider->active = true;
	else if (current_animation->GetCurrentFrameNumber() != attack_frame && player->attack_collider->active == true)
		player->attack_collider->active = false;
	if (current_animation->Finished() == true)
		OnExit();

	return UPDATE_CONTINUE;
}

void Player_AttackState::OnExit()
{
	attacking = false;
	player->attack_collider->active = false;

	current_animation->Reset();
	player->current_state = player->idle;
	player->entity_rect = player->idle->initial_rect;
}

Player_DamageState::Player_DamageState(Player* player, const char* high_frame, const char* low_frame) : PlayerState(player)
{
	App->parser->GetRect(high_rect, high_frame);
	App->parser->GetRect(low_rect, low_frame);
	float time = App->parser->GetFloat("Time_DamageState");

	in_damaged_state = new Timer((Uint32)(time * 1000.0f));
}

PlayerState* Player_DamageState::HandleInput()
{
	in_damaged_state->Start();

	return this;
}

update_status Player_DamageState::Update()
{
	if (in_damaged_state->MaxTimeReached())
		OnExit();
	else
		player->entity_rect = *damage_rect;

	return UPDATE_CONTINUE;
}

void Player_DamageState::OnExit()
{
	in_damaged_state->Stop();
	player->current_state = player->idle;
	player->entity_rect = player->idle->initial_rect;
	player->collider->active = true;
}