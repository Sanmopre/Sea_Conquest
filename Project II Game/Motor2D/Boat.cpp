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
	if (!selected)
		App->render->AddBlitEvent(1, nullptr, 0, 0, r, false, 0.0f, 0u, 0u, 255u);
	else
		App->render->AddBlitEvent(1, nullptr, 0, 0, r, false, 0.0f, 0u, 255u, 0u);
}

void Boat::CleanUp()
{

}