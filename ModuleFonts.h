#ifndef MODULEFONTS_H
#define MODULEFONTS_H

#include "Module.h"
#include "Point.h"
#include <vector>

#define MODULE_FONTS "ModuleFonts"
#define FONTS_SECTION "Config.Modules.Fonts"

struct SDL_Texture;
struct SDL_Rect;

struct TextLine
{
	const char* line;
	int x, y;
	int id_font;
};

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

	int Load(const char* order, int char_width, int font_height, const iPoint& font_origin);

	bool BlitScreenXCentered(int id_font, int y, const char* string) { return BlitScreenXCentered(id_font, y, string, fDEFAULT_SPEED); }
	bool BlitScreenXCentered(int id_font, int y, const char* string, float speed);

	bool BlitFromXCenter(int id_font, const iPoint& center_origin, const char* string) { return BlitFromXCenter(id_font, center_origin, string, fDEFAULT_SPEED); }
	bool BlitFromXCenter(int id_font, const iPoint& center_origin, const char* string, float speed);

	bool BlitFromRight(int id_font, const iPoint& right_origin, const char* string) { return BlitFromRight(id_font, right_origin, string, fDEFAULT_SPEED); }
	bool BlitFromRight(int id_font, const iPoint& right_origin, const char* string, float speed);

	bool Blit(int id_font, const iPoint& origin, const char* string) { return Blit(id_font, origin, string, fDEFAULT_SPEED); }
	bool Blit(int id_font, const iPoint& origin, const char* string, float speed);

	void FillTextLine(TextLine* line, int num_line);

private:
	SDL_Texture* texture;
	std::vector<Font*> fonts;

	int num_fonts = 0;
	float fDEFAULT_SPEED = 1.0f;
};



#endif // !MODULEFONTS_H