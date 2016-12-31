#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"
#include "ModuleFadeToBlack.h"
#include "ModuleScenePreStage.h"
#include "SDL/include/SDL_scancode.h"
#include "JsonHandler.h"
#include "Timer.h"

ModuleScenePreStage::ModuleScenePreStage(JSONParser* parser, bool active) : Module(MODULESCENE_PRESTAGE, active), current_state(HISTORY1)
{
	if (parser->LoadObject(SCENE_SECTION_PRESTAGE))
	{
		parser->GetAnimation(animated_point, "Point_Animation");
		parser->UnloadObject();
	}
	timer = new Timer();
}

ModuleScenePreStage::~ModuleScenePreStage()
{
	RELEASE(timer);
}

bool ModuleScenePreStage::Start()
{
	bool ret = true;

	LOG("Loading pre-stage scene");

	App->renderer->camera.x = App->renderer->camera.y = 0;

	switch (current_state)
	{
	case HISTORY1:
		if (App->parser->LoadObject(SCENE_SECTION_HISTORY1))
		{
			texture = App->textures->Load(App->parser->GetString("TexturePath"));
			App->parser->GetRect(rect_map, "BackRect");
			App->parser->GetPoint(ipos_map, "BackPos");
			ffade_time = abs(App->parser->GetFloat("FadeTime"));
			float seconds = abs(App->parser->GetFloat("TimeSeconds"));
			int ilines = App->parser->GetInt("NumTextLines");
			App->parser->LoadArrayInObject("TextLines");
			for (int i = 0; i < ilines; i++)
			{
				lines.push_back(new TextLine());
				App->fonts->FillTextLine(lines.back(), i);
			}
			ret = App->parser->UnloadObject();

			if (ret == true)
			{
				seconds -= ffade_time;
				seconds = (seconds > 0.0f) ? seconds : ffade_time;
				timer->Start((Uint32)seconds * 1000);
			}
		}
		App->audio->StopMusic();
		break;
	case HISTORY2:
	case HISTORY3:
		if (App->parser->LoadObject(SCENE_SECTION_HISTORY2))
		{
			texture = App->textures->Load(App->parser->GetString("TexturePath"));
			App->parser->GetRect(rect_map, "BackRect");
			App->parser->GetPoint(ipos_map, "BackPos");
			ffade_time = abs(App->parser->GetFloat("FadeTime"));
			float seconds = abs(App->parser->GetFloat("TimeSeconds"));
			int ilines = App->parser->GetInt("NumTextLines");
			ilines_print = App->parser->GetInt("FirstPrint");
			App->parser->LoadArrayInObject("TextLines");
			for (int i = 0; i < ilines; i++)
			{
				lines.push_back(new TextLine());
				App->fonts->FillTextLine(lines.back(), i);
			}
			ret = App->parser->UnloadObject();

			if (ret == true)
			{
				seconds -= ffade_time;
				seconds = (seconds > 0.0f) ? seconds : ffade_time;
				timer->Start((Uint32)seconds * 1000);
			}
		}
		break;
	case STAGE3:
		if (App->parser->LoadObject(SCENE_SECTION_PRESTAGE3))
		{
			texture = App->textures->Load(App->parser->GetString("TexturePath"));
			ffade_time = abs(App->parser->GetFloat("FadeTime"));
			float seconds = abs(App->parser->GetFloat("TimeSeconds"));
			float music_fade = App->parser->GetFloat("MusicFade");
			const char* music_string = App->parser->GetString("MusicPath");

			App->parser->GetRect(rect_map, "MapRect");
			App->parser->GetRect(rect_mission, "MissionRect");
			App->parser->GetRect(rect_country, "CountryRect");
			App->parser->GetPoint(ipos_map, "MapPos");
			App->parser->GetPoint(ipos_mission, "MissionPos");
			App->parser->GetPoint(ipos_country, "CountryPos");
			App->parser->GetPoint(ipos_point, "PointOnMap");
			ret = App->parser->UnloadObject();

			if (ret == true)
			{
				seconds -= ffade_time;
				seconds = (seconds > 0.0f) ? seconds : ffade_time;
				timer->Start((Uint32)seconds * 1000);
				App->audio->PlayMusic(music_string, music_fade);
				ipos_point.y += ipos_map.y;
				ipos_point.x += (App->window->GetScreenWidth() - rect_map.w) / 2;
			}
		}
		
		break;
	case PRESTAGE_UNKNOWN:
		break;
	default:
		break;
	}

	return ret;
}

bool ModuleScenePreStage::CleanUp()
{
	LOG("Unloading pre-stage scene");

	App->textures->Unload(texture);

	switch (current_state)
	{
	case HISTORY1:
		timer->Stop();
		for (std::vector<TextLine*>::iterator it = lines.begin(); it != lines.end(); ++it)
			delete(*it);
		lines.clear();
		current_state = HISTORY2;
		break;
	case HISTORY2:
	case HISTORY3:
		timer->Stop();
		for (std::vector<TextLine*>::iterator it = lines.begin(); it != lines.end(); ++it)
			RELEASE(*it);
		lines.clear();
		current_state = STAGE3;
		break;
	case STAGE3:
		break;
	case PRESTAGE_UNKNOWN:
		break;
	default:
		break;
	}

	return true;
}

update_status ModuleScenePreStage::Update()
{
	switch (current_state)
	{
	case HISTORY1:
		App->renderer->BlitXCentered(texture, ipos_map.y, &rect_map);
		for (std::vector<TextLine*>::iterator it = lines.begin(); it != lines.end(); ++it)
			App->fonts->BlitXCentered((*it)->id_font, (*it)->y, (*it)->line);
		if ((App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || timer->MaxTimeReached() == true) && App->fade->isFading() == false)
			App->fade->FadeToBlack(this, this, ffade_time);
		break;
	case HISTORY2:
		App->renderer->BlitXCentered(texture, ipos_map.y, &rect_map);
		for (int i = 0; i < ilines_print; i++)
			if (lines[i] != nullptr)
				App->fonts->BlitXCentered(lines[i]->id_font, lines[i]->y, lines[i]->line);

		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && App->fade->isFading() == false)
			App->fade->FadeToBlack(this, this, ffade_time);
		if (timer->MaxTimeReached() == true && App->fade->isFading() == false)
		{
			timer->Reset();
			current_state = HISTORY3;
		}
		break;
	case HISTORY3:
		App->renderer->BlitXCentered(texture, ipos_map.y, &rect_map);
		for (int i = ilines_print; i < lines.size(); i++)
			if (lines[i] != nullptr)
				App->fonts->BlitXCentered(lines[i]->id_font, lines[i]->y, lines[i]->line);
		if ((App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || timer->MaxTimeReached() == true) && App->fade->isFading() == false)
			App->fade->FadeToBlack(this, this, ffade_time);
		break;
	case STAGE3:
		App->renderer->BlitXCentered(texture, ipos_map.y, &rect_map);
		App->renderer->BlitXCentered(texture, ipos_mission.y, &rect_mission);
		App->renderer->BlitXCentered(texture, ipos_country.y, &rect_country);
		App->renderer->Blit(texture, ipos_point, &(animated_point.GetCurrentFrame()));
		break;
	case PRESTAGE_UNKNOWN:
		break;
	default:
		break;
	}

	return UPDATE_CONTINUE;
}

