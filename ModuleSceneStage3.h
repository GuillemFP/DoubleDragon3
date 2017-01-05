#ifndef MODULESCENESTAGE3_H
#define MODULESCENESTAGE3_H

#include "Module.h"
#include "Point.h"

#define MODULESCENE_STAGE3 "ModuleScene_Stage3"
#define SCENE_SECTION_STAGE3 "Config.Scenes.Stage3"

#define ENTITY_PLAYER1 "Config.Entities.Player1"
#define ENTITY_STORESIGN "Config.Entities.StoreSign"

struct SDL_Texture;
class Player;
class Room;

class ModuleSceneStage3 : public Module
{
public:
	ModuleSceneStage3(bool active);
	~ModuleSceneStage3();

	bool Start();
	update_status Update();
	bool CleanUp();

private:
	SDL_Texture* background = nullptr;
	SDL_Texture* players = nullptr;
	SDL_Texture* signals = nullptr;

	SDL_Rect rect_back = { 0,0,0,0 };
	
	iPoint ipos_back = { 0,0 };

	Player* player_one = nullptr;
	
	Room* outside = nullptr;
	Room* inside = nullptr;
	Room* current_room = nullptr;
};



#endif // !MODULESCENESTAGE3_H