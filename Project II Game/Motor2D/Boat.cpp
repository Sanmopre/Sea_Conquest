#include "j1Entities.h"

#include "j1Render.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include <vector>

Boat::Boat(int x, int y, int level, int team)
{
	type = Entity_Type::BOAT;
	position.x = x;
	position.y = y;
	destination = position;
	this->level = level;
	this->team = team;
	range = 100;
	max_health = 100;
	health = max_health;
	rect = { position.x, position.y, 20, 20 };
}

Boat::~Boat()
{

}

void Boat::Update(float dt)
{
	showing_hpbar = false;
	rect.x = position.x;
	rect.y = position.y;

	if (!selected)
	{
		if (team == 0)
			color.Blue();
		else if (team == 1)
			color.Red();
	}
	else
	{
		if (team == 0)
			color.SetColor(0u, 255u, 255u);
		else if (team == 1)
			color.SetColor(255u, 255u, 0u);
		

		if (App->input->GetMouseButtonDown(3) == KEY_DOWN)
			SetDestination();

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			Attack();

		ShowHPbar(10, 5);
	}

	Move();

	FindTarget();

	App->render->AddBlitEvent(1, nullptr, 0, 0, rect, false, 0.0f, color.r, color.g, color.b, color.a);

	if (health == 0)
		CleanUp();
}

void Boat::CleanUp()
{
	to_delete = true;
}

void  Boat::Move()
{
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

void  Boat::SetDestination()
{
	App->input->GetMousePosition(destination.x, destination.y);
	destination.x -= App->render->camera.x / App->win->GetScale();
	destination.y -= App->render->camera.y / App->win->GetScale();	
}

void  Boat::Attack()
{
	if (target != nullptr)
		Damage(10, target);
}

void  Boat::FindTarget()
{
	bool first = false;
	for (std::vector<Entity*>::iterator e = App->entitymanager->entities.begin(); e != App->entitymanager->entities.end(); e++)
	{
		if (*e != this && (*e)->team != team)
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

				ShowHPbar(10, 5);
			}
	}
	if (!first)
		target = nullptr;
}
