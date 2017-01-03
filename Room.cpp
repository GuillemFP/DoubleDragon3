#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "Room.h"
#include "JsonHandler.h"
#include "Point.h"

Room::Room(SDL_Texture* texture, const char* name) : Entity(Entity::Type::ROOM, texture)
{
	if (App->parser->LoadObject(name))
	{
		iPoint position = { 0,0 };
		App->parser->GetRect(back_rect, "Rectangle");
		App->parser->GetPoint(position, "Position");
		float fdepth = App->parser->GetFloat("Z");

		if (App->parser->UnloadObject())
		{
			x = position.x;
			y = position.y;
			z = (int)(fdepth * App->window->GetScreenHeight());
			width = back_rect.w;
			height = back_rect.h;
		}
	}
}

Room::~Room()
{
}

update_status Room::Update()
{
	App->renderer->Blit(texture, {x,y}, &back_rect);

	return UPDATE_CONTINUE;
}