#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class PlayerState;
class Player_MoveState;
class Player_StandState;
class ModuleStages;

class Player : public Entity
{
public:
	enum XDirection { LEFT = -1, XIDLE = 0, RIGHT = 1 };
	enum ZDirection { DOWN = -1, YIDLE = 0, UP = 1 };
	enum Attack { PUNCH, KICK, NOATTACK };

public:
	Player(int number_player, SDL_Texture* texture, const char* name, Entity* parent = nullptr);
	~Player();
	update_status PreUpdate();
	update_status Update();

public:
	int ispeed;
	bool inverted = false;
	bool running = false;
	int number_player = 1;

	XDirection xmovement = XIDLE;
	ZDirection zmovement = YIDLE;
	Attack attack_cmd = NOATTACK;
	bool jump = false;

	ZDirection last_zmov = YIDLE;

	Player_MoveState* moving;
	Player_StandState* idle;
	PlayerState* current_state;
};

#endif // PLAYER_H