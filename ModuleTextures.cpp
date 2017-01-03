#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "SDL/include/SDL.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

ModuleTextures::ModuleTextures() : Module(MODULE_TEXTURES)
{
}

ModuleTextures::~ModuleTextures()
{
	IMG_Quit();
}

bool ModuleTextures::Init()
{
	LOG("Init Image library");
	bool ret = true;

	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleTextures::CleanUp()
{
	LOG("Freeing textures and Image library");

	for (std::list<SDL_Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
		SDL_DestroyTexture(*it);

	textures.clear();
	return true;
}

SDL_Texture* const ModuleTextures::Load(const char* path)
{
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface;

	if (path != nullptr)
	{
		surface = IMG_Load(path);
		if (surface == nullptr)
		{
			LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
		}
		else
		{
			texture = SDL_CreateTextureFromSurface(App->renderer->renderer, surface);

			if (texture == nullptr)
			{
				LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
			}
			else
			{
				textures.push_back(texture);
			}

			SDL_FreeSurface(surface);
		}
	}
	else
		LOG("Texture path is NULL");

	return texture;
}

void ModuleTextures::Unload(SDL_Texture* texture)
{
	for (std::list<SDL_Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		if (*it == texture)
		{
			SDL_DestroyTexture(*it);
			textures.erase(it);
			break;
		}
	}
}