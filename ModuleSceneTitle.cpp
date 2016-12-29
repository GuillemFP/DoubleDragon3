#include "Application.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneTitle.h"
#include "Animation.h"
#include "SDL/include/SDL_scancode.h"
#include "JsonHandler.h"

ModuleSceneTitle::ModuleSceneTitle(JSONParser* parser, bool active) : Module(MODULESCENE_TITLE, active)
{
	animated_title.LoadAnimation(parser, MODULESCENE_FIRST_ANIMATION);
}

ModuleSceneTitle::~ModuleSceneTitle()
{
}

bool ModuleSceneTitle::Start()
{
	LOG("Loading title scene");

	switch (current_state)
	{
	case LOGO1:
		if (App->parser->LoadObject(MODULESCENE_FIRST))
		{
			texture = App->textures->Load(App->parser->GetString("TexturePath"));
			App->parser->UnloadObject();
		}
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
			App->parser->UnloadObject();
		}
		break;
	case TITLE:
		break;
	default:
		break;
	}

	App->renderer->camera.x = App->renderer->camera.y = 0;

	return true;
}

bool ModuleSceneTitle::CleanUp()
{
	LOG("Unloading title scene");

	switch (current_state)
	{
	case LOGO1:
		previous_screen = LOGO1;
		current_state = LOGO2;
		break;
	case LOGO2:
		previous_screen = LOGO2;
		current_state = LOGO1;
		break;
	case TITLE:
		previous_screen = TITLE;
		current_state = UNKNOWN;
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
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && App->fade->isFading() == false)
			App->fade->FadeToBlack(this, this);
		break;
	case LOGO2:
		App->renderer->DrawQuad(App->renderer->camera, backcolor_r, backcolor_g, backcolor_b, backcolor_a, false);
		App->renderer->BlitCentered(texture, NULL);
		break;
	case TITLE:
		break;
	default:
		break;
	}

	

	return UPDATE_CONTINUE;
}

