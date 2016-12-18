#ifndef APPLICATION_H
#define APPLICATION_H

#include <list>
#include "Globals.h"
#include "Module.h"

class ModuleWindow;
class ModuleRender;

class Application
{
public:
	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleWindow* window;
	ModuleRender* renderer;

private:
	std::list<Module*> modules;
};

extern Application* App;

#endif // !APPLICATION_H