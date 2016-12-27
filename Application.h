#ifndef APPLICATION_H
#define APPLICATION_H

#include <list>
#include "Globals.h"
#include "Module.h"

class ModuleInput;
class ModuleWindow;
class ModuleRender;
class ModuleTextures;
class ModuleAudio;

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

private:
	std::list<Module*> modules;
};

extern Application* App;

#endif // !APPLICATION_H
