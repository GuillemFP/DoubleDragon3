#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneTitle.h"
#include "ModuleEntities.h"
#include "SDL/include/SDL_scancode.h"
#include "JsonHandler.h"
#include "Timer.h"

ModuleSceneTitle::ModuleSceneTitle(JSONParser* parser, bool active) : Module(MODULESCENE_TITLE, active), current_state(LOGO1)
{
	if (parser->LoadObject(SCENE_SECTION_FIRST))
	{
		parser->GetAnimation(animated_title, "SegaLogo_Animation");
		parser->UnloadObject();
	}
	timer = new Timer();
}

ModuleSceneTitle::~ModuleSceneTitle()
{
	RELEASE(timer);
}

bool ModuleSceneTitle::Start()
{
	bool ret = true;

	LOG("Loading title scene");

	App->renderer->camera.x = App->renderer->camera.y = 0;

	switch (current_state)
	{
	case LOGO1:
		if (App->parser->LoadObject(SCENE_SECTION_FIRST))
		{
			texture = App->textures->Load(App->parser->GetString("TexturePath"));
			ffade_time = abs(App->parser->GetFloat("FadeTime"));
			float seconds = abs(App->parser->GetFloat("TimeSeconds"));
			float music_fade = App->parser->GetFloat("MusicFade");
			const char* music_string = App->parser->GetString("MusicPath");
			ret = App->parser->UnloadObject();

			if (ret == true)
			{
				seconds -= ffade_time;
				seconds = (seconds > 0.0f) ? seconds : ffade_time;
				timer->Start((Uint32)seconds * 1000);
				App->audio->PlayMusic(music_string, music_fade);
			}
		}
		else
			ret = false;
		break;
	case LOGO2:
		if (App->parser->LoadObject(SCENE_SECTION_SECOND))
		{
			texture = App->textures->Load(App->parser->GetString("TexturePath"));
			App->parser->LoadArrayInObject("BackgroundColor");
			backcolor_r = App->parser->GetIntFromArray(0);
			backcolor_g = App->parser->GetIntFromArray(1);
			backcolor_b = App->parser->GetIntFromArray(2);
			backcolor_a = App->parser->GetIntFromArray(3);
			ffade_time = abs(App->parser->GetFloat("FadeTime"));
			float seconds = abs(App->parser->GetFloat("TimeSeconds"));
			ret = App->parser->UnloadObject();

			if (ret == true)
			{
				seconds -= ffade_time;
				seconds = (seconds > 0.0f) ? seconds : ffade_time;
				timer->Start((Uint32)seconds * 1000);
			}
		}
		else
			ret = false;
		break;
	case TITLE_SCROLL:
		if (App->parser->LoadObject(SCENE_SECTION_THIRD))
		{
			texture = App->textures->Load(App->parser->GetString("TexturePath"));
			App->parser->GetRect(rect_back, "BackgroundRect");
			App->parser->GetRect(rect_title, "TitleRect");
			int speed = abs(App->parser->GetInt("TitleSpeed"));
			fspeed_title = -(float)speed;
			ispeed_camera = abs(App->parser->GetInt("CameraSpeed")) * App->window->GetScreenSize();
			ffade_time = abs(App->parser->GetFloat("TransitionTime"));
			float music_fade = App->parser->GetFloat("MusicFade");
			const char* music_string = App->parser->GetString("MusicPath");
			ret = App->parser->UnloadObject();

			if (ret == true)
			{
				ifinalpos_cam = App->window->GetScreenWidth() - rect_back.w;
				ipos_background = { 0,0 };
				ipos_title = { (ifinalpos_cam * speed + (App->window->GetScreenWidth() - rect_title.w) / 2), 0 };
				ifinalpos_cam *= App->window->GetScreenSize();
				if (App->audio->isPlayingMusic() == false)
					App->audio->PlayMusic(music_string, music_fade);
			}
		}
		else
			ret = false;
		break;
	case TITLE:
		if (App->parser->LoadObject(SCENE_SECTION_THIRD))
		{
			texture = App->textures->Load(App->parser->GetString("TexturePath"));
			App->parser->GetRect(rect_back, "BackgroundRect");
			App->parser->GetRect(rect_title, "TitleRect");
			App->parser->GetRect(rect_subtitle, "SubtitleRect");
			ffade_time = abs(App->parser->GetFloat("FadeTime"));
			y_text = App->parser->GetInt("PositionYText");
			font_text = App->parser->GetInt("FontText");
			string_text = App->parser->GetString("StringTextTitle");
			ret = App->parser->UnloadObject();
			
			if (ret == true)
			{
				ipos_background = { App->window->GetScreenWidth() - rect_back.w,0 };
				ipos_title = { (App->window->GetScreenWidth() - rect_title.w) / 2,0 };
				ipos_subtitle = { (App->window->GetScreenWidth() - rect_title.w) / 2,rect_title.h };
			}
		}
		else
			ret = false;
		App->entities->ResetCoins();
		break;
	default:
		ret = false;
		break;
	}

	return ret;
}

bool ModuleSceneTitle::CleanUp()
{
	LOG("Unloading title scene");

	switch (current_state)
	{
	case LOGO1:
		timer->Stop();
		current_state = LOGO2;
		break;
	case LOGO2:
		timer->Stop();
		current_state = TITLE_SCROLL;
		break;
	case TITLE_SCROLL:
		current_state = TITLE;
		break;
	case TITLE:
		current_state = TITLE_SCROLL;
		break;
	}

	App->textures->Unload(texture);

	return true;
}

update_status ModuleSceneTitle::Update()
{
	switch (current_state)
	{
	case LOGO1:
		App->renderer->BlitScreenCentered(texture, &(animated_title.GetCurrentFrame()));
		if (timer->MaxTimeReached() == true && App->fade->isFading() == false)
			App->fade->FadeToBlack(this, this, ffade_time);
		break;
	case LOGO2:
		App->renderer->DrawQuad(App->renderer->camera, backcolor_r, backcolor_g, backcolor_b, backcolor_a, false);
		App->renderer->BlitScreenCentered(texture, NULL);
		if ((App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || timer->MaxTimeReached() == true) && App->fade->isFading() == false)
			App->fade->FadeToBlack(this, this, ffade_time);
		break;
	case TITLE_SCROLL:
		if (App->renderer->camera.x > ifinalpos_cam)
			App->renderer->camera.x -= ispeed_camera;
		App->renderer->Blit(texture, ipos_background, &rect_back);
		App->renderer->Blit(texture, ipos_title, &rect_title, fspeed_title);
		if ((App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->renderer->camera.x <= ifinalpos_cam) && App->fade->isFading() == false)
			App->fade->FadeToWhite(this, this, ffade_time);
		break;
	case TITLE:
		App->renderer->Blit(texture, ipos_background, &rect_back);
		App->renderer->Blit(texture, ipos_title, &rect_title);
		App->renderer->Blit(texture, ipos_subtitle, &rect_subtitle);
		App->fonts->BlitScreenXCentered(font_text, y_text, string_text);
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && App->fade->isFading() == false)
			App->fade->FadeToBlack((Module*)App->scene_prestage, this);
		break;
	default:
		break;
	}

	return UPDATE_CONTINUE;
}

