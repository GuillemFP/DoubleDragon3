#ifndef PLAYERSTATES_H
#define PLAYERSTATES_H

#include "Animation.h"
#include "SDL/include/SDL_rect.h"

class Player;

class PlayerState
{
public:
	PlayerState(Player* player) : player(player) {}
	virtual ~PlayerState() {}
	virtual update_status Update() { return UPDATE_CONTINUE; }
	virtual PlayerState* HandleInput() { return nullptr; }

protected:
	Player* player;
};

class Player_StandState : public PlayerState
{
public:
	Player_StandState(Player* player, const char* staticframe);
	~Player_StandState();
	PlayerState* HandleInput();
	update_status Update();

public:
	SDL_Rect current_rect = { 0,0 };
	SDL_Rect initial_rect = { 0,0 };
	PlayerState* previous_state = this;
};



class Player_MoveState : public PlayerState
{
public:
	Player_MoveState(Player* player, const char* move_animation, const char* moveup_animation);
	~Player_MoveState();
	PlayerState* HandleInput();
	update_status Update();

public:
	Animation moving;
	Animation moving_up;
	Animation* current_animation = nullptr;
	SDL_Rect current_rect = { 0,0 };
};
#endif // !PLAYERSTATES_H