#include "j1Entities.h"

#include "j1Render.h"

Boat::Boat()
{
	type = Entity_Type::BOAT;
	position.x = 0.0f;
	position.y = 0.0f;
	level = 1;
}

Boat::Boat(float x, float y, int level)
{
	type = Entity_Type::BOAT;
	position.x = x;
	position.y = y;
	this->level = level;
}

Boat::~Boat()
{

}

void Boat::Update(float dt)
{
	SDL_Rect r = { position.x, position.y, 20, 20 };
	if(!selected)
		App->render->DrawQuad(r, 0,0,200);
	else
		App->render->DrawQuad(r, 0, 200, 0);
}

void Boat::CleanUp()
{

}