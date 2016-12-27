#ifndef MODULETEXTURES_H
#define MODULETEXTURES_H

#include <list>
#include "Module.h"

#define MODULE_TEXTURES "ModuleTextures"

struct SDL_Texture;

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();
	
	bool Init();
	bool CleanUp();

	SDL_Texture* const Load(const char* path);
	void Unload(SDL_Texture* texture);

private:
	std::list<SDL_Texture*> textures;
};


#endif // !MODULETEXTURES_H