#ifndef MODULEFONTS_H
#define MODULEFONTS_H

#include "Module.h"
#include "Point.h"
#include <vector>

#define MODULE_FONTS "ModuleFonts"
#define FONTS_SECTION "Config.Modules.Fonts"

struct SDL_Texture;
struct SDL_Rect;

struct Font
{
	int id_font;
	int char_width;
	char* table_char;
	std::vector<SDL_Rect> textures_char;
};

class ModuleFonts : public Module
{
public:
	ModuleFonts();
	~ModuleFonts();

	bool Start();
	bool CleanUp();

	int Load(const char* order, int char_width, int font_height, iPoint font_origin);
	bool Blit(int id_font, iPoint origin, const char* string, float speed);

private:
	

private:
	SDL_Texture* texture;
	std::vector<Font*> fonts;
	int num_fonts = 0;
};



#endif // !MODULEFONTS_H