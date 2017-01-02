#ifndef MODULESCENEPRESTAGE_H
#define MODULESCENEPRESTAGE_H

#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include <vector>

#define MODULESCENE_PRESTAGE "ModuleScene_PreStage"
#define SCENE_SECTION_PRESTAGE "Config.Scenes.PreStage"
#define SCENE_SECTION_HISTORY1 "Config.Scenes.PreStage.History1"
#define SCENE_SECTION_HISTORY2 "Config.Scenes.PreStage.History23"
#define SCENE_SECTION_PRESTAGE3 "Config.Scenes.PreStage.Stage3"

struct SDL_Texture;
struct TextLine;
class JSONParser;
class Timer;

class ModuleScenePreStage : public Module
{
private:
	enum PreStageScreen
	{
		HISTORY1,
		HISTORY2,
		HISTORY3,
		STAGE3,
		UNKNOWN
	};

public:
	ModuleScenePreStage(JSONParser* parser, bool active = true);
	~ModuleScenePreStage();

	bool Start();
	update_status Update();
	bool CleanUp();

private:
	SDL_Texture* texture = nullptr;

	Animation animated_point;
	Animation face1;
	Animation face2;

	SDL_Rect rect_map = { 0,0,0,0 };
	SDL_Rect rect_mission = { 0,0,0,0 };
	SDL_Rect rect_country = { 0,0,0,0 };

	iPoint ipos_map = { 0,0 };
	iPoint ipos_mission = { 0,0 };
	iPoint ipos_country = { 0,0 };

	iPoint ipos_point = { 0,0 };
	iPoint ipos_face1 = { 0,0 };
	iPoint ipos_face2 = { 0,0 };

	PreStageScreen current_state = UNKNOWN;

	std::vector<TextLine*> lines;
	int ilines_print;

	Timer* timer = nullptr;
	float ffade_time = 1.0f;
};



#endif // !MODULESCENEPRESTAGE_H