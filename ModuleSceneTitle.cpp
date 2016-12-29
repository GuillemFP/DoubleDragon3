#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneTitle.h"
#include "Animation.h"
#include "SDL/include/SDL_scancode.h"
#include "JsonHandler.h"

ModuleSceneTitle::ModuleSceneTitle(JSONParser* parser, bool active) : Module(MODULESCENE_TITLE, active), current_state(LOGO1)
{
	if (parser->LoadObject(MODULESCENE_FIRST))
	{
		parser->GetAnimation(animated_title, "SegaLogo_Animation");
		parser->UnloadObject();
	}
}

ModuleSceneTitle::~ModuleSceneTitle()
{
}

bool ModuleSceneTitle::Start()
{
	bool ret = true;

	LOG("Loading title scene");

	App->renderer->camera.x = App->renderer->camera.y = 0;

	switch (current_state)
	{
	case LOGO1:
		if (App->parser->LoadObject(MODULESCENE_FIRST))
		{
			texture = App->textures->Load(App->parser->GetString("TexturePath"));
			ffade_time = abs(App->parser->GetFloat("FadeTime"));
			float seconds = abs(App->parser->GetFloat("TimeSeconds"));
			ret = App->parser->UnloadObject();

			seconds -= ffade_time;
			seconds = (seconds > 0.0f) ? seconds : ffade_time;
			timer.Start((Uint32)seconds * 1000);
		}
		else
			ret = false;
		App->audio->PlayMusic("assets/mus_rosetta-stone.ogg");
		break;
	case LOGO2:
		if (App->parser->LoadObject(MODULESCENE_SECOND))
		{
			texture = App->textures->Load(App->parser->GetString("TexturePath"));
			App->parser->LoadArrayInObject("BackgroundColor");
			backcolor_r = App->parser->GetValueFromArray(0);
			backcolor_g = App->parser->GetValueFromArray(1);
			backcolor_b = App->parser->GetValueFromArray(2);
			backcolor_a = App->parser->GetValueFromArray(3);
			ffade_time = abs(App->parser->GetFloat("FadeTime"));
			float seconds = abs(App->parser->GetFloat("TimeSeconds"));
			ret = App->parser->UnloadObject();

			seconds -= ffade_time;
			seconds = (seconds > 0.0f) ? seconds : ffade_time;
			timer.Start((Uint32)seconds * 1000);
		}
		else
			ret = false;
		break;
	case TITLE_SCROLL:
		if (App->parser->LoadObject(MODULESCENE_THIRD))
		{
			texture = App->textures->Load(App->parser->GetString("TexturePath"));
			App->parser->GetRect(background, "BackgroundRect");
			App->parser->GetRect(title, "TitleRect");
			int speed = abs(App->parser->GetInt("TitleSpeed"));
			fspeed_title = -(float)speed;
			ispeed_camera = abs(App->parser->GetInt("CameraSpeed")) * App->window->GetScreenSize();
			ffade_time = abs(App->parser->GetFloat("FadeTime"));
			ret = App->parser->UnloadObject();

			ifinalpos_cam = App->window->GetScreenWidth() - background.w;
			ipos_background = { 0,0 };
			ipos_title = { (ifinalpos_cam * speed + (App->window->GetScreenWidth() - title.w) / 2), 0 };
			ifinalpos_cam *= App->window->GetScreenSize();
		}
		else
			ret = false;
		break;
	case TITLE:
		if (App->parser->LoadObject(MODULESCENE_THIRD))
		{
			texture = App->textures->Load(App->parser->GetString("TexturePath"));
			App->parser->GetRect(background, "BackgroundRect");
			App->parser->GetRect(title, "TitleRect");
			App->parser->GetRect(subtitle, "SubtitleRect");
			ret = App->parser->UnloadObject();

			ipos_background = { App->window->GetScreenWidth() - background.w,0 };
			ipos_title = { (App->window->GetScreenWidth() - title.w) / 2,0 };
			ipos_subtitle = { (App->window->GetScreenWidth() - title.w) / 2,title.h };
		}
		else
			ret = false;
		break;
	default:
		break;
	}

	return true;
}

bool ModuleSceneTitle::CleanUp()
{
	LOG("Unloading title scene");

	switch (current_state)
	{
	case LOGO1:
		timer.Stop();
		current_state = LOGO2;
		break;
	case LOGO2:
		timer.Stop();
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
		App->renderer->BlitCentered(texture, &(animated_title.GetCurrentFrame()));
		if (timer.MaxTimeReached() == true && App->fade->isFading() == false)
			App->fade->FadeToBlack(this, this, ffade_time);
		break;
	case LOGO2:
		App->renderer->DrawQuad(App->renderer->camera, backcolor_r, backcolor_g, backcolor_b, backcolor_a, false);
		App->renderer->BlitCentered(texture, NULL);
		if ((App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || timer.MaxTimeReached() == true) && App->fade->isFading() == false)
			App->fade->FadeToBlack(this, this, ffade_time);
		break;
	case TITLE_SCROLL:
		if (App->renderer->camera.x > ifinalpos_cam)
			App->renderer->camera.x -= ispeed_camera;
		App->renderer->Blit(texture, ipos_background, &background);
		App->renderer->Blit(texture, ipos_title, &title, fspeed_title);
		if (App->renderer->camera.x <= ifinalpos_cam && App->fade->isFading() == false)
			App->fade->FadeToBlack(this, this, ffade_time);
		break;
	case TITLE:
		App->renderer->Blit(texture, ipos_background, &background);
		App->renderer->Blit(texture, ipos_title, &title);
		App->renderer->Blit(texture, ipos_subtitle, &subtitle);
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && App->fade->isFading() == false)
			App->fade->FadeToBlack(this, this);
		break;
	default:
		break;
	}

	

	return UPDATE_CONTINUE;
}
