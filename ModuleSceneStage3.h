#ifndef MODULESCENESTAGE3_H
#define MODULESCENESTAGE3_H

#include "Module.h"
#include "ModuleStages.h"
#include "Point.h"

#define MODULESCENE_STAGE3 "ModuleScene_Stage3"
#define SCENE_SECTION_STAGE3 "Config.Scenes.Stage3"

#define ENTITY_SAMURAI "Config.Entities.Samurai"
#define ENTITY_STORESIGN "Config.Entities.StoreSign"

struct SDL_Texture;
class Player;
class Room;
class Object;
class Creature;
class Timer;

class ModuleSceneStage3 : public ModuleStages
{
public:
	ModuleSceneStage3(bool active);
	~ModuleSceneStage3();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

	void SpawnEnemies();

	bool InsideScene_LeftBorder(const Point3d& positions, const Point3d& dimensions, bool in_plataform = false) const;
	bool InsideScene_RightBorder(const Point3d& positions, const Point3d& dimensions, bool in_plataform = false) const;
	bool InsideScene_LowBorder(const Point3d& positions, const Point3d& dimensions, bool in_plataform = false) const;
	bool InsideScene_HighBorder(const Point3d& positions, const Point3d& dimensions, bool in_plataform = false) const;

	bool InPlataform(int x, int z) const;

private:
	SDL_Texture* background = nullptr;

	SDL_Rect rect_back = { 0,0,0,0 };
	
	iPoint ipos_back = { 0,0 };

	iPoint plataform_min = { 0,0 };
	iPoint plataform_max = { 0,0 };

	Player* player_one = nullptr;
	
	Room* outside = nullptr;
	Room* inside = nullptr;
	Room* current_room = nullptr;

	Object* door = nullptr;

	int* borders_xmin = nullptr;
	int* borders_xmax = nullptr;
	int* borders_zmin = nullptr;
	int* borders_zmin_in_plataform = nullptr;
	int borders_zmax = 0;
	
};



#endif // !MODULESCENESTAGE3_H