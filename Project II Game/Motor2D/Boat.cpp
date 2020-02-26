#include "j1Entities.h"

#include "j1Render.h"
#include "j1Window.h"
#include "j1Input.h"

Boat::Boat()
{
	type = Entity_Type::BOAT;
	position.x = 0.0f;
	position.y = 0.0f;
	destination = position;
	level = 1;
}

Boat::Boat(float x, float y, int level)
{
	type = Entity_Type::BOAT;
	position.x = x;
	position.y = y;
	destination = position;
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
	{
		if (App->input->GetMouseButtonDown(3) == KEY_DOWN)
		{
			App->input->GetMousePosition(destination.x, destination.y);
			destination.x -= App->render->camera.x / App->win->GetScale();
			destination.y -= App->render->camera.y / App->win->GetScale();
		}
		App->render->AddBlitEvent(1, nullptr, 0, 0, r, false, 0.0f, 0u, 255u, 0u);
	}
	if (destination != position)
	{
		if (position.x < destination.x)
			position.x++;
		if (position.x > destination.x)
			position.x--;
		if (position.y < destination.y)
			position.y++;
		if (position.y > destination.y)
			position.y--;
	}
}

void Boat::CleanUp()
{

}