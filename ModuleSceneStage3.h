#ifndef MODULESCENESTAGE3_H
#define MODULESCENESTAGE3_H

#include "Module.h"
#include "SDL/include/SDL_rect.h"
#include "Point.h"

#define MODULESCENE_STAGE3 "ModuleScene_Stage3"
#define SCENE_SECTION_STAGE3 "Config.Scenes.Stage3"

struct SDL_Texture;

class ModuleSceneStage3 : public Module
{
public:
	ModuleSceneStage3(bool active);
	~ModuleSceneStage3();

	bool Start();
	update_status Update();
	bool CleanUp();

private:
	SDL_Texture* texture = nullptr;

	SDL_Rect rect_back = { 0,0,0,0 };
	
	iPoint ipos_back = { 0,0 };
};



#endif // !MODULESCENESTAGE3_H