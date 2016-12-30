#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
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

	/*Load(" 0123456789abcdefghijklmnopqrstuvwxyz?-", 8, 8, { 0,0 });*/

	return ret;
}

bool ModuleFonts::CleanUp()
{
	App->textures->Unload(texture);

	for (std::vector<Font*>::iterator it = fonts.begin(); it != fonts.end(); ++it)
	{
		RELEASE((*it)->table_char);
		RELEASE(*it)
	}

	return true;
}

int ModuleFonts::Load(const char * order, int char_width, int font_height, iPoint font_origin)
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

bool ModuleFonts::Blit(int id_font, iPoint origin, const char* string, float speed)
{
	bool ret = true;
	char* match;
	int char_pos;
	Font* font;
	iPoint relative;

	if (id_font < fonts.size())
	{
		font = fonts[id_font];
		for (int i = 0; i < strlen(string); i++)
		{
			match = strchr(font->table_char, string[i]);
			if (match != nullptr)
			{
				char_pos = (int) (match - font->table_char);
				relative = {(font->char_width)*i, 0};
				App->renderer->Blit(texture, origin + relative, &(font->textures_char[char_pos]), speed);
			}
			else
			{
				LOG("Error localizing character %c in font id %i", string[i], id_font);
				ret = false;
			}
		}
	}
	else
	{
		LOG("Unknown font id: %i", id_font);
		ret = false;
	}

	return ret;
}
