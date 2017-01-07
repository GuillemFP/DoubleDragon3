#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleFonts.h"
#include "JsonHandler.h"

ModuleFonts::ModuleFonts() : Module(MODULE_FONTS)
{
}

ModuleFonts::~ModuleFonts()
{
}

bool ModuleFonts::Start()
{
	bool ret = true;

	LOG("Loading fonts");

	if (App->parser->LoadObject(FONTS_SECTION))
	{
		fDEFAULT_SPEED = App->parser->GetFloat("DefaultBlitSpeed");
		texture = App->textures->Load(App->parser->GetString("TexturePath"));
		int num_fonts = App->parser->GetInt("NumFonts");
		App->parser->LoadArrayInObject("Fonts");
		iPoint font_origin;
		for (int i = 0; i < num_fonts; i++)
		{
			font_origin = { App->parser->GetIntFromArrayInArray(i,3),App->parser->GetIntFromArrayInArray(i,4) };
			Load(App->parser->GetStringFromArrayInArray(i, 0), App->parser->GetIntFromArrayInArray(i, 1), App->parser->GetIntFromArrayInArray(i, 2), font_origin);
		}
		App->parser->UnloadObject();
	}

	return ret;
}

bool ModuleFonts::CleanUp()
{
	App->textures->Unload(texture);

	for (std::vector<Font*>::iterator it = fonts.begin(); it != fonts.end(); ++it)
	{
		RELEASE_ARRAY((*it)->table_char);
		RELEASE(*it)
	}

	return true;
}

int ModuleFonts::Load(const char * order, int char_width, int font_height, const iPoint& font_origin)
{
	Font* font = new Font;
	SDL_Rect rect;

	font->id_font = num_fonts;
	font->char_width = char_width;

	font->table_char = new char[strlen(order) + 1];
	memcpy(font->table_char, order, strlen(order) + 1);
	fonts.push_back(font);

	for (int i = 0; i < strlen(order); i++)
	{
		rect = { font_origin.x + i*char_width,font_origin.y,char_width,font_height };
		font->textures_char.push_back(rect);
	}

	return num_fonts++;
}

bool ModuleFonts::BlitScreenXCentered(int id_font, int y, const char * string, float speed)
{
	iPoint origin;

	origin.x = App->window->GetScreenWidth()/2 - strlen(string)*fonts[id_font]->char_width/2;
	origin.y = y;

	return Blit(id_font, origin, string, speed);
}

bool ModuleFonts::BlitFromXCenter(int id_font, const iPoint & center_origin, const char * string, float speed)
{
	iPoint left_origin = center_origin;
	left_origin.x -= strlen(string)*fonts[id_font]->char_width/2;

	return Blit(id_font, left_origin, string, speed);
}

bool ModuleFonts::BlitFromRight(int id_font, const iPoint & right_origin, const char * string, float speed)
{
	iPoint left_origin = right_origin;
	left_origin.x -= strlen(string)*fonts[id_font]->char_width;

	return Blit(id_font, left_origin, string, speed);
}

bool ModuleFonts::Blit(int id_font, const iPoint& origin, const char* string, float speed)
{
	bool ret = true;
	Font* font;
	char* match;
	iPoint relative;

	if (id_font < fonts.size())
	{
		font = fonts[id_font];
		for (int i = 0; i < strlen(string); i++)
		{
			match = strchr(font->table_char, string[i]);
			if (match != nullptr)
			{
				relative = { (font->char_width)*i, 0 };
				if (App->renderer->Blit(texture, origin + relative, &(font->textures_char[(int)(match - font->table_char)]), speed) == false)
					return false;
			}
			else
			{
				LOG("Error localizing character %c in font id %i", string[i], id_font);
				return false;
			}
		}
	}
	else
	{
		LOG("Unknown font id: %i", id_font);
		return false;
	}

	return ret;
}

void ModuleFonts::FillTextLine(TextLine* line, int num_line)
{
	line->id_font = App->parser->GetIntFromArrayInArray(num_line, 0);
	line->x = App->parser->GetIntFromArrayInArray(num_line, 1);
	line->y = App->parser->GetIntFromArrayInArray(num_line, 2);
	line->line = App->parser->GetStringFromArrayInArray(num_line, 3);
}
