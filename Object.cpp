#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "Object.h"
#include "JsonHandler.h"
#include "Point.h"

Object::Object(SDL_Texture* texture, const char* name, Entity* parent) : Entity(Entity::Type::ROOM, texture, parent)
{
	if (App->parser->LoadObject(name))
	{
		iPoint position = { 0,0 };
		App->parser->GetRect(object_rect, "Rectangle");
		App->parser->GetPoint(position, "Position");
		float fdepth = App->parser->GetFloat("Z");

		if (App->parser->UnloadObject())
		{
			x = position.x;
			y = position.y;
			z = (int)(fdepth * App->window->GetScreenHeight());
			width = object_rect.w;
			height = object_rect.h;
		}
	}
}

Object::~Object()
{
}

update_status Object::Update()
{
	if (parent->active == true)
		App->renderer->Blit(texture, { x,y }, &object_rect);

	return UPDATE_CONTINUE;
}