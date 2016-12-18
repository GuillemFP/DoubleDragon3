#include "Application.h"

Application::Application()
{
	
	
}

Application::~Application()
{
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);
}

bool Application::Init()
{
	bool ret = true;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if ((*it)->IsEnabled())
			ret = (*it)->Start();
	}

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