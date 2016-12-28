#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "SDL/include/SDL.h"

ModuleRender::ModuleRender() : Module(MODULE_RENDER)
{
}

ModuleRender::~ModuleRender()
{}

bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;
	Uint32 flags = 0;

	if (ConstantConfig() == false)
	{
		LOG("Problem retrieving value from configuration file");
		ret = false;
	}

	camera.w = App->window->GetScreenWidth() * iSCREENSIZE;
	camera.h = App->window->GetScreenHeight() * iSCREENSIZE;

	if (bVSYNC)
		flags |= SDL_RENDERER_PRESENTVSYNC;

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);

	if (renderer == nullptr)
	{
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleRender::PreUpdate()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	return UPDATE_CONTINUE;
}

update_status ModuleRender::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_RenderPresent(renderer);
	return UPDATE_CONTINUE;
}

bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	if (renderer != nullptr)
	{
		SDL_DestroyRenderer(renderer);
	}

	return true;
}

bool ModuleRender::BlitCentered(SDL_Texture* texture, SDL_Rect* section)
{
	return BlitCentered(texture, section, fDEFAULT_SPEED);
}

bool ModuleRender::BlitCentered(SDL_Texture* texture, SDL_Rect* section, float speed)
{
	iPoint draw_origin;

	if (section != nullptr)
	{
		draw_origin.x = 0.5*iSCREENWIDTH - 0.5*section->w;
		draw_origin.y = 0.5*iSCREENHEIGHT - 0.5*section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &draw_origin.x, &draw_origin.y);
		draw_origin.x = 0.5*iSCREENWIDTH - 0.5*draw_origin.x;
		draw_origin.y = 0.5*iSCREENHEIGHT - 0.5*draw_origin.y;
	}

	return Blit(texture, draw_origin, section, speed);
}

bool ModuleRender::Blit(SDL_Texture* texture, iPoint position, SDL_Rect* section)
{
	return Blit(texture, position, section, fDEFAULT_SPEED);
}

bool ModuleRender::Blit(SDL_Texture* texture, iPoint position, SDL_Rect* section, float speed)
{
	bool ret = true;
	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + position.x * iSCREENSIZE;
	rect.y = (int)(camera.y * speed) + position.y * iSCREENSIZE;

	if (section != nullptr)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);

	rect.w *= iSCREENSIZE;
	rect.h *= iSCREENSIZE;

	if (SDL_RenderCopy(renderer,texture,section,&rect) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (use_camera == true)
	{
		rec.x = (int)(camera.x + rect.x * iSCREENSIZE);
		rec.y = (int)(camera.y + rect.y * iSCREENSIZE);
		rec.w *= iSCREENSIZE;
		rec.h *= iSCREENSIZE;
	}

	if (SDL_RenderFillRect(renderer,&rec) != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::ConstantConfig()
{
	bool ret = true;

	bVSYNC = JSON_GetBool(VSYNC);
	ret = JSON_GetFloat(DEFAULT_SPEED, fDEFAULT_SPEED);

	iSCREENSIZE = App->window->GetScreenSize();
	iSCREENWIDTH = App->window->GetScreenWidth();
	iSCREENHEIGHT = App->window->GetScreenHeight();

	return ret;
}