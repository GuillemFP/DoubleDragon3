#include "Application.h"
#include "ModuleEntities.h"
#include "ModuleCollision.h"
#include "ModuleAudio.h"
#include "Enemy.h"
#include "EnemyStates.h"
#include "Player.h"
#include "JsonHandler.h"

Enemy_StandState::Enemy_StandState(Enemy* enemy, const char * staticframe) : EnemyState(enemy)
{
	App->parser->GetRect(initial_rect, staticframe);
	current_rect = initial_rect;
	App->parser->GetRect(current_collider, "MoveCollider");
}

EnemyState * Enemy_StandState::Logic()
{
	return enemy->moving->Logic();
}

update_status Enemy_StandState::Update()
{
	enemy->entity_rect = current_rect;
	enemy->SetCollider(current_collider);
	return UPDATE_CONTINUE;
}

void Enemy_StandState::OnExit()
{
}

Enemy_MoveState::Enemy_MoveState(Enemy* enemy, const char* move_animation, const char* moveup_animation) : EnemyState(enemy)
{
	App->parser->GetAnimation(moving, move_animation);
	App->parser->GetAnimation(moving_up, moveup_animation);
	App->parser->GetRect(collider_moving, "MoveCollider");
	App->parser->GetRect(collider_moving_up, "MoveUpCollider");
	current_animation = &moving;
	current_collider = &collider_moving;
}

EnemyState * Enemy_MoveState::Logic()
{
	EnemyState* ret = this;

	if (enemy->current_target != nullptr)
	{
		enemy->target_xdirection = enemy->RelativeDirectionXTo(enemy->current_target);
		switch (enemy->target_xdirection)
		{
		case Creature::XDirection::LEFT:
			enemy->inverted_texture = true;
			break;
		case Creature::XDirection::RIGHT:
			enemy->inverted_texture = false;
			break;
		}
		enemy->target_zdirection = enemy->RelativeDirectionZTo(enemy->current_target);
	}

	if (enemy->logic_timer->MaxTimeReached() || enemy->logic_timer->GetState() == TimerState::OFF)
	{
		enemy->running = false;
		enemy->logic_timer->Reset();
		enemy->current_target = App->entities->GetTargetPlayer(enemy);
		Enemy::TargetState current_state = enemy->GetCurrentTargetState();
		switch (current_state)
		{
		case Enemy::CLOSE:
			if (enemy->SetSlot() == true)
			{
				if (enemy->AttackChoice() == true)
					ret = enemy->attacking;
			}
			else
				enemy->SetDestinationInClose();
			break;
		case Enemy::ENGAGED:
			if (enemy->current_target->hasFreeSlots())
				enemy->SetDestinationOnPlayer();
			else
				enemy->SetDestinationInEngage();
			break;
		case Enemy::DETECTED:
			enemy->keeping_distance = false;
			enemy->running = true;
			enemy->SetDestinationOnPlayer();
			break;
		case Enemy::FAR:
			ret = enemy->idle;
			break;
		case Enemy::TARGETLESS:
			ret = enemy->idle;
			break;
		default:
			break;
		}
	}

	if (enemy->current_target == nullptr)
		ret = enemy->idle;

	return ret;
}

update_status Enemy_MoveState::Update()
{
	Creature::XDirection xdirection = enemy->RelativeDirectionXTo(enemy->final_destination->x);
	Creature::ZDirection zdirection = enemy->RelativeDirectionZTo(enemy->final_destination->y);
	bool jumping = false;

	switch (zdirection)
	{
	case Creature::UP:
		current_animation = &moving_up;
		moving.Reset();
		current_collider = &collider_moving_up;
		if (enemy->stage->InsideScene_HighBorder(enemy->position, enemy->dimensions, enemy->in_plataform))
		{
			enemy->position.z -= enemy->ispeed;
			enemy->position.y -= enemy->ispeed;
		}
		else if (enemy->in_plataform == false && enemy->stage->InPlataform(*(enemy->final_destination)) == true)
			jumping = true;
		else if (enemy->in_plataform == false && enemy->current_target->in_plataform == true)
			jumping = true;
		break;
	case Creature::DOWN:
		current_animation = &moving;
		moving_up.Reset();
		current_collider = &collider_moving;
		if (enemy->stage->InsideScene_LowBorder(enemy->position, enemy->dimensions, enemy->in_plataform))
		{
			enemy->position.z += enemy->ispeed;
			enemy->position.y += enemy->ispeed;
		}
		else if (enemy->in_plataform == true && enemy->stage->InPlataform(*(enemy->final_destination)) == false)
			jumping = true;
		else if (enemy->in_plataform == true && enemy->current_target->in_plataform == false)
			jumping = true;
		break;
	}

	switch (xdirection)
	{
	case Creature::LEFT:
		if (enemy->stage->InsideScene_LeftBorder(enemy->position, enemy->dimensions, enemy->in_plataform))
		{
			enemy->position.x -= enemy->ispeed;
			if (enemy->running)
			{
				if (enemy->stage->InsideScene_LeftBorder(enemy->position, enemy->dimensions, enemy->in_plataform))
					enemy->position.x -= enemy->ispeed;
				else if (enemy->in_plataform == true && enemy->stage->InPlataform(*(enemy->final_destination)) == false)
					jumping = true;
				else if (enemy->in_plataform == true && enemy->current_target->in_plataform == false)
					jumping = true;
			}
		}
		else if (enemy->in_plataform == true && enemy->stage->InPlataform(*(enemy->final_destination)) == false)
			jumping = true;
		else if (enemy->in_plataform == true && enemy->current_target->in_plataform == false)
			jumping = true;
		break;
	case Creature::RIGHT:
		if (enemy->stage->InsideScene_RightBorder(enemy->position, enemy->dimensions, enemy->in_plataform))
		{
			enemy->position.x += enemy->ispeed;
			if (enemy->running)
			{
				if (enemy->stage->InsideScene_RightBorder(enemy->position, enemy->dimensions, enemy->in_plataform))
					enemy->position.x += enemy->ispeed;
				else if (enemy->in_plataform == false && enemy->stage->InPlataform(*(enemy->final_destination)) == true)
					jumping = true;
				else if (enemy->in_plataform == false && enemy->current_target->in_plataform == true)
					jumping = true;
			}
		}
		else if (enemy->in_plataform == false && enemy->stage->InPlataform(*(enemy->final_destination)) == true)
			jumping = true;
		else if (enemy->in_plataform == false && enemy->current_target->in_plataform == true)
			jumping = true;
		break;
	}

	enemy->idle->current_rect = current_animation->GetCurrentFrame();
	enemy->entity_rect = enemy->idle->current_rect;
	
	enemy->SetCollider(*current_collider);
	enemy->idle->current_collider = *current_collider;

	if (jumping == true)
	{
		enemy->current_state = enemy->jumping;
		enemy->xmovement = xdirection;
		enemy->zmovement = zdirection;
	}
	else if (enemy->DestinationReached())
		enemy->current_state = enemy->idle;

	return UPDATE_CONTINUE;
}

void Enemy_MoveState::OnExit()
{
}

Enemy_JumpState::Enemy_JumpState(Enemy* enemy, const char* jump_frame, const char* jumpattack_frame) : EnemyState(enemy)
{
	App->parser->GetRect(jump_rect, jump_frame);
	App->parser->GetRect(jumpkick_rect, jumpattack_frame);
	App->parser->GetPoint(speed, "JumpSpeed");
	gravity = App->parser->GetFloat("Gravity");

	App->parser->GetRect(jump_collider, "JumpCollider");
	App->parser->GetRect(aerialkick_collider, "AerialKickCollider");
	App->parser->GetRect(attack_collider, "AerialKickAttackCollider");
	kick_damage = App->parser->GetInt("AerialKickDamage");
}

EnemyState* Enemy_JumpState::Logic()
{
	if (time == 0)
		SetJumpParameters();
	else if (attacking == false)
	{
		if (enemy->TargetWithinAttackRange() && enemy->current_target->ColliderIsActive())
		{
			attacking = true;
			enemy->entity_rect = jumpkick_rect;
			enemy->SetAttackDamage(kick_damage);
			enemy->EnableAttackCollider();
		}
	}

	return this;
}

update_status Enemy_JumpState::Update()
{
	current_position.y = (float)(initial_pos.y) - speed.y*time + gravity*time*time;
	if (enemy->position.y < ((int)(current_position.y + 0.5f)))
		maximum_reached = true;

	enemy->position.y = (int)(current_position.y + 0.5f);

	switch (jump_direction)
	{
	case Creature::XDirection::LEFT:
		enemy->inverted_texture = true;
		if (enemy->stage->InsideScene_LeftBorder(enemy->position, enemy->dimensions))
		{
			current_position.x += jump_direction*speed.x;
			enemy->position.x = (int)(current_position.x + 0.5f);
		}
		break;
	case Creature::XDirection::RIGHT:
		enemy->inverted_texture = false;
		if (enemy->stage->InsideScene_RightBorder(enemy->position, enemy->dimensions, enemy->in_plataform))
		{
			current_position.x += jump_direction*speed.x;
			enemy->position.x = (int)(current_position.x + 0.5f);
		}
		else if (direct_jump == true)
		{
			current_position.x += jump_direction*speed.x;
			enemy->position.x = (int)(current_position.x + 0.5f);
		}
		else if (jumping_to_platform == true && initial_pos.y - enemy->position.y > enemy->stage->plataform_height)
		{
			current_position.x += jump_direction*speed.x;
			enemy->position.x = (int)(current_position.x + 0.5f);
		}
		break;
	}

	if (attacking)
	{
		enemy->SetCollider(aerialkick_collider);
		enemy->SetAttackCollider(attack_collider);
	}
	else
		enemy->SetCollider(jump_collider);

	time++;

	if (enemy->position.y >= final_pos.y && maximum_reached == true)
	{
		enemy->position.y = final_pos.y;
		if (jumping_to_platform)
			enemy->in_plataform = true;
		else if (jumping_off_platform)
			enemy->in_plataform = false;
		OnExit();
	}

	return UPDATE_CONTINUE;
}

void Enemy_JumpState::OnExit()
{
	time = 0;
	enemy->current_state = enemy->idle;

	enemy->ResetCollider();
	enemy->logic_timer->Reset();
	if (attacking == true)
	{
		attacking = false;
		enemy->DisableAttackCollider();
	}
}

void Enemy_JumpState::SetJumpParameters()
{
	time = 1;
	initial_pos.x = enemy->position.x;
	initial_pos.y = enemy->position.y;
	final_pos.x = initial_pos.x;
	final_pos.y = initial_pos.y;
	current_position.x = (float)(initial_pos.x);
	current_position.y = (float)(initial_pos.y);

	jump_direction = enemy->xmovement;
	direct_jump = false;
	jumping_to_platform = false;
	jumping_off_platform = false;
	maximum_reached = false;
	attacking = false;

	final_pos.x += (int)(this->enemy->xmovement*speed.x*speed.y / gravity);

	switch (enemy->xmovement)
	{
	case Creature::XDirection::RIGHT:
		if (enemy->stage->InPlataform(final_pos.x + enemy->dimensions.x / 2, enemy->position.z) == true &&
			enemy->in_plataform == false)
		{
			final_pos.y -= enemy->stage->plataform_height;
			jumping_to_platform = true;
		}
		else if (enemy->zmovement == Creature::ZDirection::UP &&
			enemy->stage->InPlataform(final_pos.x + enemy->dimensions.x / 2, enemy->position.z - 1) == true &&
			enemy->in_plataform == false)
		{
			enemy->position.z -= 1;
			final_pos.y -= enemy->stage->plataform_height + 1;
			jumping_to_platform = true;
			direct_jump = true;
		}
		else if (enemy->zmovement == Creature::ZDirection::DOWN &&
			enemy->stage->InPlataform(final_pos.x + enemy->dimensions.x / 2, enemy->position.z) == false &&
			enemy->in_plataform == true)
		{
			final_pos.y += enemy->stage->plataform_height;
			jumping_off_platform = true;
		}
		break;
	case Creature::XDirection::LEFT:
		if (enemy->stage->InPlataform(final_pos.x, enemy->position.z) == false && enemy->in_plataform == true)
		{
			final_pos.y += enemy->stage->plataform_height;
			jumping_off_platform = true;
		}
		else if (enemy->zmovement == Creature::ZDirection::UP &&
			enemy->stage->InPlataform(final_pos.x + enemy->dimensions.x / 2, enemy->position.z - 1) == true &&
			enemy->in_plataform == false)
		{
			enemy->position.z -= 1;
			final_pos.y -= enemy->stage->plataform_height + 1;
			jumping_to_platform = true;
			direct_jump = true;
		}
		else if (enemy->zmovement == Creature::ZDirection::DOWN &&
			enemy->stage->InPlataform(final_pos.x + enemy->dimensions.x / 2, enemy->position.z) == false &&
			enemy->in_plataform == true)
		{
			final_pos.y += enemy->stage->plataform_height;
			jumping_off_platform = true;
		}
		break;
		break;
	case Creature::XDirection::XIDLE:
		if (enemy->zmovement == Creature::ZDirection::DOWN &&
			enemy->stage->InPlataform(final_pos.x, enemy->position.z) == false &&
			enemy->in_plataform == true)
		{
			final_pos.y += enemy->stage->plataform_height;
			jumping_off_platform = true;
		}
		if (enemy->zmovement == Creature::ZDirection::UP &&
			enemy->stage->InPlataform(final_pos.x + enemy->dimensions.x, enemy->position.z - 1) == true &&
			enemy->in_plataform == false)
		{
			enemy->position.z -= 1;
			final_pos.y -= enemy->stage->plataform_height + 1;
			jumping_to_platform = true;
		}
	}

	enemy->entity_rect = jump_rect;
}

Enemy_AttackState::Enemy_AttackState(Enemy* enemy, const char* punch_animation, const char* kick_animation) : EnemyState(enemy)
{
	App->parser->GetAnimation(punch, punch_animation);
	punch_damage = App->parser->GetInt("PunchDamage");
	punch_frame = App->parser->GetInt("PunchDamageFrame");
	App->parser->GetRect(punch_rect, "PunchRect");
	animation_colliders = new SDL_Rect[punch.GetNumberOfFrames()];
	shift_frames = new iPoint[punch.GetNumberOfFrames()];
	App->parser->LoadArrayInObject("PunchAnimationColliders");
	for (int i = 0; i < punch.GetNumberOfFrames(); i++)
		App->parser->GetRectFromArray(animation_colliders[i], i);
	App->parser->LoadArrayInObject("PunchShiftFrames");
	for (int i = 0; i < punch.GetNumberOfFrames(); i++)
		App->parser->GetPointFromArray(shift_frames[i], i);

	if (kick_animation != nullptr)
	{
		App->parser->GetAnimation(kick, kick_animation);
		kick_damage = App->parser->GetInt("KickDamage");
		kick_frame = App->parser->GetInt("KickDamageFrame");
		App->parser->GetRect(kick_rect, "KickRect");
	}

	current_animation = &punch;
}

EnemyState * Enemy_AttackState::Logic()
{
	if (attacking == false)
	{
		attacking = true;
		enemy->SetAttackType(Creature::Attack::PUNCH);
		current_animation = &punch;
		enemy->SetAttackDamage(punch_damage);
		attack_frame = punch_frame;
		attack_rect = &punch_rect;
		enemy->SetAttackCollider(*attack_rect);
	}
	return this;
}

update_status Enemy_AttackState::Update()
{
	enemy->entity_rect = current_animation->GetCurrentFrame();
	int frame_num = current_animation->GetCurrentFrameNumber();
	enemy->shifted_draw = shift_frames[frame_num];
	enemy->SetCollider(animation_colliders[frame_num]);
	if (frame_num == attack_frame && enemy->AttackColliderIsActive() == false)
		enemy->EnableAttackCollider();
	else if (frame_num != attack_frame && enemy->AttackColliderIsActive() == true)
		enemy->DisableAttackCollider();
	if (current_animation->Finished() == true)
		OnExit();

	return UPDATE_CONTINUE;
}

void Enemy_AttackState::OnExit()
{
	enemy->ResetShift();

	attacking = false;
	enemy->DisableAttackCollider();

	current_animation->Reset();
	enemy->current_state = enemy->idle;
	enemy->entity_rect = enemy->idle->current_rect;

	enemy->ResetCollider();
	enemy->logic_timer->Reset();
}

Enemy_DamageState::Enemy_DamageState(Enemy* enemy, const char* high_frame, const char* low_frame) : EnemyState(enemy)
{
	App->parser->GetRect(high_rect, high_frame);
	App->parser->GetRect(low_rect, low_frame);
	float time = App->parser->GetFloat("Time_DamageState");
	App->parser->GetPoint(high_shift, "DamageHigh_Shift");
	App->parser->GetPoint(low_shift, "DamageLow_Shift");

	in_damaged_state = new Timer((Uint32)(time * 1000.0f));
}

update_status Enemy_DamageState::Update()
{
	if (in_damaged_state->MaxTimeReached())
		OnExit();
	else
	{
		enemy->entity_rect = *damage_rect;
		enemy->shifted_draw = *damage_shift;
	}

	return UPDATE_CONTINUE;
}

void Enemy_DamageState::OnExit()
{
	enemy->ResetShift();
	in_damaged_state->Stop();
	enemy->current_state = enemy->idle;
	enemy->entity_rect = enemy->idle->initial_rect;
	enemy->EnableCollider();
}

Enemy_FallState::Enemy_FallState(Enemy* enemy, const char* falling_frame, const char* fallen_frame, const char* rising_frame) : EnemyState(enemy)
{
	App->parser->GetRect(falling_rect, falling_frame);
	App->parser->GetRect(fallen_rect, fallen_frame);
	App->parser->GetRect(rising_rect, rising_frame);
	App->parser->GetPoint(speed, "JumpSpeed");
	gravity = App->parser->GetFloat("Gravity");

	fallen_time = App->parser->GetFloat("Fallen_Time");
	App->parser->GetPoint(fallen_shift, "FallenShift");
	App->parser->GetPoint(rising_shift, "RisingShift");
	rising_time = App->parser->GetFloat("Rising_Time");

	states_timer = new Timer();
}

EnemyState * Enemy_FallState::Logic()
{
	switch (state)
	{
	case Enemy_FallState::FALLING:
		if (time == 0)
		{
			enemy->FreeSlot();
			SetFallParameters();
		}
		break;
	case Enemy_FallState::LYING:
		if (states_timer->GetState() == TimerState::OFF)
		{
			enemy->shifted_draw = fallen_shift;
			states_timer->SetMaxTime((Uint32)(fallen_time*1000.0f));
			states_timer->Start();
		}
		break;
	case Enemy_FallState::RISING:
		if (states_timer->GetState() == TimerState::OFF)
		{
			enemy->shifted_draw = rising_shift;
			states_timer->SetMaxTime((Uint32)(rising_time*1000.0f));
			states_timer->Start();
		}
		break;
	}

	return this;
}

update_status Enemy_FallState::Update()
{
	switch (state)
	{
	case Enemy_FallState::FALLING:
	{
		current_position.y = (float)(initial_pos.y) - speed.y*time + gravity*time*time;
		if (enemy->position.y < ((int)(current_position.y + 0.5f)))
			maximum_reached = true;

		enemy->position.y = (int)(current_position.y + 0.5f);

		switch (fall_direction)
		{
		case Creature::XDirection::LEFT:
			if (enemy->stage->InsideScene_LeftBorder(enemy->position, enemy->dimensions))
			{
				current_position.x += fall_direction*speed.x;
				enemy->position.x = (int)(current_position.x + 0.5f);
			}
			break;
		case Creature::XDirection::RIGHT:
			if (enemy->stage->InsideScene_RightBorder(enemy->position, enemy->dimensions, enemy->in_plataform))
			{
				current_position.x += fall_direction*speed.x;
				enemy->position.x = (int)(current_position.x + 0.5f);
			}
			else if (jumping_to_platform == true && initial_pos.y - enemy->position.y > enemy->stage->plataform_height)
			{
				current_position.x += fall_direction*speed.x;
				enemy->position.x = (int)(current_position.x + 0.5f);
			}
			break;
		}

		time++;

		if (enemy->position.y >= final_pos.y && maximum_reached == true)
		{
			enemy->position.y = final_pos.y;
			if (jumping_to_platform)
			{
				enemy->in_plataform = true;
			}
			else if (jumping_off_platform)
			{
				enemy->in_plataform = false;
			}
			App->audio->PlayFx(enemy->sound_jump);
			state = LYING;
		}
	}
	break;
	case Enemy_FallState::LYING:
		if (states_timer->MaxTimeReached() == true)
		{
			if (enemy->health > 0)
			{
				state = RISING;
				states_timer->Stop();
			}
			else if (enemy->dead == false)
			{
				states_timer->DoubleMaxTime();
				enemy->Dying();
			}
			else
			{
				states_timer->Stop();
				OnExit();
				enemy->DisableCollider();
				enemy->Disable();
				enemy->ChangeParent(nullptr);
				enemy->Delete();
			}
		}
		else
			enemy->entity_rect = fallen_rect;
		break;
	case Enemy_FallState::RISING:
		if (states_timer->MaxTimeReached() == true)
		{
			states_timer->Stop();
			OnExit();
		}
		else
			enemy->entity_rect = rising_rect;
		break;
	}

	return UPDATE_CONTINUE;
}

void Enemy_FallState::OnExit()
{
	time = 0;
	state = FALLING;
	states_timer->Stop();
	enemy->ResetShift();
	enemy->current_state = enemy->idle;
	enemy->idle->current_rect = enemy->idle->initial_rect;
	enemy->EnableCollider();
	enemy->accumulated_damage = 0;
}

void Enemy_FallState::SetFallParameters()
{
	state = FALLING;
	time = 1;
	initial_pos.x = enemy->position.x;
	initial_pos.y = enemy->position.y;
	final_pos.x = initial_pos.x;
	final_pos.y = enemy->position.z - enemy->dimensions.y;
	if (enemy->in_plataform)
		final_pos.y -= enemy->stage->plataform_height;
	current_position.x = (float)(initial_pos.x);
	current_position.y = (float)(initial_pos.y);

	jumping_to_platform = false;
	jumping_off_platform = false;
	maximum_reached = false;

	final_pos.x += (int)(fall_direction*speed.x*speed.y / gravity);

	switch (fall_direction)
	{
	case Creature::XDirection::RIGHT:
		if (enemy->stage->InPlataform(final_pos.x + enemy->dimensions.x / 2, enemy->position.z) == true &&
			enemy->in_plataform == false)
		{
			final_pos.y -= enemy->stage->plataform_height;
			jumping_to_platform = true;
		}
		if (enemy->stage->InPlataform(final_pos.x, enemy->position.z) == false &&
			enemy->in_plataform == true)
		{
			final_pos.y += enemy->stage->plataform_height;
			jumping_off_platform = true;
		}
		break;
	case Creature::XDirection::LEFT:
		if (enemy->stage->InPlataform(final_pos.x, enemy->position.z) == false && enemy->in_plataform == true)
		{
			final_pos.y += enemy->stage->plataform_height;
			jumping_off_platform = true;
		}
		if (enemy->stage->InPlataform(final_pos.x, enemy->position.z) == true &&
			enemy->in_plataform == false)
		{
			final_pos.y -= enemy->stage->plataform_height;
			jumping_to_platform = true;
		}
		break;
	}

	enemy->entity_rect = falling_rect;
}

