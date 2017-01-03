#ifndef APPLICATION_H
#define APPLICATION_H

#include <list>
#include "Globals.h"
#include "Module.h"

#define APP_SECTION "Config.App"

class JSONParser;

class ModuleInput;
class ModuleWindow;
class ModuleRender;
class ModuleTextures;
class ModuleAudio;
class ModuleFonts;

class ModuleSceneTitle;
class ModuleScenePreStage;
class ModuleSceneStage3;
class ModuleEntities;

class ModuleFadeToBlack;

class Application
{
public:
	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleInput* input;
	ModuleWindow* window;
	ModuleRender* renderer;
	ModuleTextures* textures;
	ModuleAudio* audio;
	ModuleFonts* fonts;

	ModuleSceneTitle* scene_title;
	ModuleScenePreStage* scene_prestage;
	ModuleSceneStage3* scene_stage3;
	ModuleEntities* entities;

	ModuleFadeToBlack* fade;

	JSONParser* parser;

private:
	std::list<Module*> modules;
	float ffade_time = 1.0f;
};

extern Application* App;

#endif // !APPLICATION_H
