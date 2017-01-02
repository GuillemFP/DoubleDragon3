#ifndef MODULESCENETITLE_H
#define MODULESCENETITLE_H

#include "Module.h"
#include "Animation.h"
#include "Point.h"

#define MODULESCENE_TITLE "ModuleScene_Title"
#define SCENE_SECTION_FIRST "Config.Scenes.Title.FirstScreen"
#define SCENE_SECTION_SECOND "Config.Scenes.Title.SecondScreen"
#define SCENE_SECTION_THIRD "Config.Scenes.Title.TitleScreen"

struct SDL_Texture;
class JSONParser;
class Timer;

class ModuleSceneTitle : public Module
{
private:
	enum TitleScreen
	{
		LOGO1,
		LOGO2,
		TITLE_SCROLL,
		TITLE,
		UNKNOWN
	};

public:
	ModuleSceneTitle(JSONParser* parser, bool active = true);
	~ModuleSceneTitle();

	bool Start();
	update_status Update();
	bool CleanUp();

private:
	SDL_Texture* texture = nullptr;

	Animation animated_title;
	SDL_Rect rect_back = { 0,0,0,0 };
	SDL_Rect rect_title = { 0,0,0,0 };
	SDL_Rect rect_subtitle = { 0,0,0,0 };

	iPoint ipos_background = { 0,0 };
	iPoint ipos_title = { 0,0 };
	iPoint ipos_subtitle = { 0,0 };

	float fspeed_title = 1.0f;
	int ispeed_camera = 1;
	int ifinalpos_cam = 0;

	Uint8 backcolor_r = 0;
	Uint8 backcolor_g = 0;
	Uint8 backcolor_b = 0;
	Uint8 backcolor_a = 0;

	TitleScreen current_state = UNKNOWN;

	Timer* timer = nullptr;
	float ffade_time = 1.0f;
};



#endif // !MODULESCENEPRETITLE_H
