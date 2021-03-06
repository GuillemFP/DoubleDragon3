#include "Application.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRender.h"
#include "SDL/include/SDL.h"
#include "JsonHandler.h"

ModuleFadeToBlack::ModuleFadeToBlack() : Module(MODULE_FADETOBLACK)
{
	fade_time = new Timer();
}

ModuleFadeToBlack::~ModuleFadeToBlack()
{
	RELEASE(fade_time);
}

bool ModuleFadeToBlack::Init()
{
	bool ret = true;

	if (ConstantConfig() == false)
	{
		LOG("Problem retrieving value from configuration file");
		ret = false;
	}

	return ret;
}

bool ModuleFadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->renderer->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

update_status ModuleFadeToBlack::Update()
{
	if (fade_time->GetState() == RUNNING)
	{
		float alpha_correction = (float)fade_time->GetTimeInMs() / (float)fade_time->GetMaxTimeInMs();

		if (alpha_correction > 1.0f)
			alpha_correction = 1.0f;

		if (fading_in == false)
			alpha_correction = 1.0f - alpha_correction;

		SDL_SetRenderDrawColor(App->renderer->renderer, fadecolor_r, fadecolor_g, fadecolor_b, (Uint8)(alpha_correction * 255.0f));
		SDL_RenderFillRect(App->renderer->renderer, NULL);

		if (module_off == nullptr && module_on != nullptr)
		{
			module_on->Enable();
			module_on = nullptr;
		}

		if (fade_time->MaxTimeReached())
		{
			if (fading_in == true)
			{
				if (module_off != nullptr)
					module_off->Disable();
				module_on->Enable();

				fade_time->Reset();
				fading_in = false;
			}
			else
			{
				fade_time->Stop();
			}
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleFadeToBlack::FadeToBlack(Module * module_on, Module * module_off, float time)
{
	fading_in = (module_off != nullptr) ? true : false;
	fade_time->Start((Uint32)(time * 0.5f * 1000.0f));

	fadecolor_r = 0;
	fadecolor_g = 0;
	fadecolor_b = 0;

	this->module_on = module_on;
	this->module_off = module_off;
}

void ModuleFadeToBlack::FadeToWhite(Module * module_on, Module * module_off, float time)
{
	fading_in = (module_off != nullptr) ? true : false;
	fade_time->Start((Uint32)(time * 0.5f * 1000.0f));

	fadecolor_r = 255;
	fadecolor_g = 255;
	fadecolor_b = 255;

	this->module_on = module_on;
	this->module_off = module_off;
}

bool ModuleFadeToBlack::ConstantConfig()
{
	bool ret = true;

	if (App->parser->LoadObject(FADETOBLACK_SECTION) == true)
	{
		fDEFAULT_TIME = App->parser->GetFloat("DefaultTime");
		ret = App->parser->UnloadObject();
	}
	else
		ret = false;

	return ret;
}
