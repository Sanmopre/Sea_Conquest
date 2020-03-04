#include "j1Entities.h"

#include "j1Render.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include <vector>
#include "j1Map.h"

Boat::Boat(int x, int y, int level, int team)
{
	type = Entity_Type::BOAT;
	position.x = x;
	position.y = y;
	destination = position;
	this->level = level;
	this->team = team;
	speed = 1;
	range = 100;
	firerate = { 1 };
	max_health = 100;
	health = max_health;
	//rect = { position.x, position.y, 20, 20 };
	for (std::vector<Animation>::iterator i = App->map->wholeAnimations.begin(); i != App->map->wholeAnimations.end(); i++) 
	{
		if (this->type == (i)->type)
		{
			this->animation = i->GetAnimation();
			break;
		}
	}
	rect = { position.x, position.y, 20, 20 };
	target = nullptr;

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
		{
			color.SetColor(0u, 255u, 255u);

			if (App->input->GetMouseButtonDown(3) == KEY_DOWN)
				SetDestination();

			//if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			//	Attack();
		}
		else if (team == 1)
		{
			color.SetColor(255u, 255u, 0u);
		}
			

		ShowHPbar(10, 5);
	}

	if (target != nullptr && dt != 0.0f)
	{
		firerate.counter++;
		if (firerate.counter >= (firerate.iterations / dt))
		{
			Attack();
			firerate.counter = 0;
		}
	}

	Move();

	FindTarget();

	//App->render->AddBlitEvent(1, animation.texture, 0, 0, animation.GetCurrentFrame(), false, 0.0f, false);
	App->render->AddBlitEvent(1, nullptr, 0, 0, rect, false, false, color.r, color.g, color.b, color.a);

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
			position.x += speed;
		if (position.x > destination.x)
			position.x -= speed;
		if (position.y < destination.y)
			position.y += speed;
		if (position.y > destination.y)
			position.y -= speed;
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
