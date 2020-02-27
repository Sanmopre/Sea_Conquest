#include "j1Entities.h"

#include "j1Render.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include <vector>

Boat::Boat()
{
	type = Entity_Type::BOAT;
	position.x = 0.0f;
	position.y = 0.0f;
	destination = position;
	level = 1;
	range = 50;
}

Boat::Boat(float x, float y, int level)
{
	type = Entity_Type::BOAT;
	position.x = x;
	position.y = y;
	destination = position;
	this->level = level;
	range = 50;
}

Boat::~Boat()
{

}

void Boat::Update(float dt)
{
	SDL_Rect rect = { position.x, position.y, 20, 20 };
	if (!selected)
	{
		r = 0u;
		g = 0u;
		b = 255u;
	}
	else
	{
		if (App->input->GetMouseButtonDown(3) == KEY_DOWN)
		{
			App->input->GetMousePosition(destination.x, destination.y);
			destination.x -= App->render->camera.x / App->win->GetScale();
			destination.y -= App->render->camera.y / App->win->GetScale();
		}
		r = 0u;
		g = 255u;
		b = 0u;
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
	for (std::vector<Entity*>::iterator e = App->entitymanager->entities.begin(); e != App->entitymanager->entities.end(); e++)
	{
		if (*e != this)
			if (position.x + range > (*e)->position.x &&
				position.x - range < (*e)->position.x &&
				position.y + range >(*e)->position.y &&
				position.y - range < (*e)->position.y)
			{
				if (selected)
				{
					r = 255u;
					g = 140u;
					b = 0u;
				}
				else
				{
					r = 255u;
					g = 0u;
					b = 0u;
				}
			}
	}
	App->render->AddBlitEvent(1, nullptr, 0, 0, rect, false, 0.0f, r, g, b);
}

void Boat::CleanUp()
{

}