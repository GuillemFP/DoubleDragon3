#ifndef MODULESCENETITLE_H
#define MODULESCENETITLE_H

#include "Module.h"
#include "Animation.h"

#define MODULESCENE_TITLE "ModuleScene_Title"
#define MODULESCENE_FIRST "Config.Scenes.Title.FirstScreen"
#define MODULESCENE_FIRST_ANIMATION "Config.Scenes.Title.FirstScreen.SegaLogo_Animation"
#define MODULESCENE_SECOND "Config.Scenes.Title.SecondScreen"

class SDL_Texture;
class JSONParser;

enum TitleScreen
{
	UNKNOWN,
	LOGO1,
	LOGO2,
	TITLE
};

class ModuleSceneTitle : public Module
{
public:
	ModuleSceneTitle(JSONParser* parser, bool active = true);
	~ModuleSceneTitle();

	bool Start();
	update_status Update();
	bool CleanUp();

private:
	SDL_Texture* texture = nullptr;

	Animation animated_title;

	Uint8 backcolor_r = 0;
	Uint8 backcolor_g = 0;
	Uint8 backcolor_b = 0;
	Uint8 backcolor_a = 0;

	TitleScreen current_state = LOGO1;
	TitleScreen previous_screen = UNKNOWN;
};



#endif // !MODULESCENEPRETITLE_H
