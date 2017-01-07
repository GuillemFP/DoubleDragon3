#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"
#include "ModuleCollision.h"
#include "ModuleUI.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneTitle.h"
#include "ModuleScenePreStage.h"
#include "ModuleSceneStage3.h"
#include "ModuleEntities.h"
#include "JsonHandler.h"

Application::Application()
{
	parser = new JSONParser(CONFIGJSON);

	modules.push_back(input = new ModuleInput(parser));
	modules.push_back(window = new ModuleWindow());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(audio = new ModuleAudio());
	modules.push_back(fonts = new ModuleFonts());

	modules.push_back(entities = new ModuleEntities());

	modules.push_back(scene_title = new ModuleSceneTitle(parser, false));
	modules.push_back(scene_prestage = new ModuleScenePreStage(parser, false));
	modules.push_back(scene_stage3 = new ModuleSceneStage3(false));

	modules.push_back(collision = new ModuleCollision());
	modules.push_back(user_interface = new ModuleUI(false));
	modules.push_back(fade = new ModuleFadeToBlack());
}

Application::~Application()
{
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);

	RELEASE(parser);
}

bool Application::Init()
{
	bool ret = true;

	if (parser->LoadObject(APP_SECTION))
	{
		ffade_time = abs(parser->GetFloat("InitialFadeTime"));
		ret = parser->UnloadObject();
	}
	else
		ret = false;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if ((*it)->IsEnabled())
			ret = (*it)->Start();
	}

	fade->FadeToBlack(scene_stage3, ffade_time);

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->IsEnabled())
			ret = (*it)->PreUpdate();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->IsEnabled())
			ret = (*it)->Update();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->IsEnabled())
			ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (std::list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if ((*it)->IsEnabled())
			ret = (*it)->CleanUp();

	return ret;
}