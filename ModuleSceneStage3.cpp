#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleSceneStage3.h"
#include "JsonHandler.h"

ModuleSceneStage3::ModuleSceneStage3(bool active) : Module(MODULESCENE_STAGE3, active)
{
}

ModuleSceneStage3::~ModuleSceneStage3()
{
}

bool ModuleSceneStage3::Start()
{
	bool ret = true;

	LOG("Loading Stage3 scene");

	App->renderer->camera.x = App->renderer->camera.y = 0;

	if (App->parser->LoadObject(SCENE_SECTION_STAGE3))
	{
		texture = App->textures->Load(App->parser->GetString("TexturePath"));
		App->parser->GetRect(rect_back, "BackRect");
		App->parser->GetPoint(ipos_back, "BackPos");
		float music_fade = App->parser->GetFloat("MusicFade");
		const char* music_string = App->parser->GetString("MusicPath");
		ret = App->parser->UnloadObject();

		if (ret == true)
		{
			if (App->audio->isPlayingMusic() == false)
				App->audio->PlayMusic(music_string, music_fade);
		}
	}

	return ret;
}

bool ModuleSceneStage3::CleanUp()
{
	LOG("Unloading pre-stage scene");

	App->textures->Unload(texture);

	return true;
}

update_status ModuleSceneStage3::Update()
{

	App->renderer->Blit(texture, ipos_back, &rect_back);

	return UPDATE_CONTINUE;
}
