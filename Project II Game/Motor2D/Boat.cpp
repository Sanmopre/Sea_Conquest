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
	range = 100;
	max_health = 100;
	health = max_health;
}

Boat::Boat(float x, float y, int level)
{
	type = Entity_Type::BOAT;
	position.x = x;
	position.y = y;
	destination = position;
	this->level = level;
	range = 100;
	max_health = 100;
	health = max_health;
}

Boat::~Boat()
{

}

void Boat::Update(float dt)
{
	SDL_Rect rect = { position.x, position.y, 20, 20 };
	if (!selected)
	{
		color.SetColor(0u, 0u, 255u);
	}
	else
	{
		if (App->input->GetMouseButtonDown(3) == KEY_DOWN)
		{
			App->input->GetMousePosition(destination.x, destination.y);
			destination.x -= App->render->camera.x / App->win->GetScale();
			destination.y -= App->render->camera.y / App->win->GetScale();
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			if (target != nullptr)
				Damage(10, target);
		color.SetColor(0u, 255u, 0u);
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

	bool first = false;
	for (std::vector<Entity*>::iterator e = App->entitymanager->entities.begin(); e != App->entitymanager->entities.end(); e++)
	{
		if (*e != this)
			if (position.x + range > (*e)->position.x &&
				position.x - range < (*e)->position.x &&
				position.y + range >(*e)->position.y &&
				position.y - range < (*e)->position.y)
			{
				if (!first)
				{
					first = true;
					target = *e;
				}
				if (selected)
				{
					color.SetColor(255u, 140u, 0u);
				}
				else
				{
					color.SetColor(255u, 0u, 0u);
				}

				SDL_Rect health_rect = { rect.x - 10, rect.y -20, rect.w +20, rect.h - 15 };
				Color health_color(96u, 96u, 96u);

				App->render->AddBlitEvent(2, nullptr, 0, 0, health_rect, false, 0.0f, health_color.r, health_color.g, health_color.b, health_color.a);

				float hrw = health_rect.w;
				hrw /= max_health;
				hrw *= health;
				health_rect.w = hrw;
				health_color.SetColor(0u, 204u, 0u);

				App->render->AddBlitEvent(2, nullptr, 0, 0, health_rect, false, 0.0f, health_color.r, health_color.g, health_color.b, health_color.a);
			}
	}
	if (!first)
		target = nullptr;

	App->render->AddBlitEvent(1, nullptr, 0, 0, rect, false, 0.0f, color.r, color.g, color.b, color.a);

	if (health == 0)
		to_delete = true;
}

void Boat::CleanUp()
{
	App->entitymanager->DeleteEntity(this);
}
