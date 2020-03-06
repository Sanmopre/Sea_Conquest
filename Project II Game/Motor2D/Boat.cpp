#include "j1Entities.h"

#include "j1Render.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include <vector>
#include "j1Map.h"

Boat::Boat(float x, float y, int level, int team)
{
	type = Entity_Type::BOAT;
	position.x = x;
	position.y = y;
	destination = position;
	this->level = level;
	this->team = team;
	speed = 100;
	range = 100;
	firerate = { 1 };
	max_health = 100;
	health = max_health;
	if (true)
		LOG("I have crippling depression"); LOG("I have even more crippling depression");
	for (std::vector<Animation>::iterator i = App->map->allAnimations.begin(); i != App->map->allAnimations.end(); i++) 
	{
		if (this->type == (i)->type)
		{
			if ((i)->orientation == Orientation::NORTH)
			{
				this->north = i->GetAnimation(); continue;
			}
			if ((i)->orientation == Orientation::NORTH_EAST)
			{
				this->north_east = i->GetAnimation(); continue;
			}
			if ((i)->orientation == Orientation::EAST)
			{
				this->east = i->GetAnimation(); continue;
			}
			if ((i)->orientation == Orientation::SOUTH_EAST)
			{
				this->south_east = i->GetAnimation(); continue;
			}
			if ((i)->orientation == Orientation::SOUTH)
			{
				this->south = i->GetAnimation(); continue;
			}
			if ((i)->orientation == Orientation::SOUTH_WEST)
			{
				this->south_west = i->GetAnimation(); continue;
			}
			if ((i)->orientation == Orientation::WEST)
			{
				this->west = i->GetAnimation(); continue;
			}
			if ((i)->orientation == Orientation::NORTH_WEST)
			{
				this->north_west = i->GetAnimation(); continue;
			}
		}
	}
	rect = { (int)position.x, (int)position.y, 20, 20 };
	target = nullptr;

}

Boat::~Boat()
{

}

void Boat::Update(float dt)
{
	showing_hpbar = false;
	switch (orientation)
	{
	case Orientation::NORTH:
	{
		rect = north.GetCurrentFrame(); texture = north.texture; break;
	}
	case Orientation::NORTH_EAST:
	{
		rect = north_east.GetCurrentFrame(); texture = north_east.texture; break;
	}
	case Orientation::EAST:
	{
		rect = east.GetCurrentFrame(); texture = east.texture; break;
	}
	case Orientation::SOUTH_EAST:
	{
		rect = south_east.GetCurrentFrame(); texture = south_east.texture; break;
	}
	case Orientation::SOUTH:
	{
		rect = south.GetCurrentFrame(); texture = south.texture; break;
	}
	case Orientation::SOUTH_WEST:
	{
		rect = south_west.GetCurrentFrame(); texture = south_west.texture; break;
	}
	case Orientation::WEST:
	{
		rect = west.GetCurrentFrame(); texture = west.texture; break;
	}
	case Orientation::NORTH_WEST:
	{
		rect = north_west.GetCurrentFrame(); texture = north_west.texture; break;
	}
	}
	
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
		}
		else if (team == 1)
		{
			color.SetColor(255u, 255u, 0u);
		}
			

		ShowHPbar(10, 5);
	}

	if (destination != position)
		Move(dt);
	else if (target != nullptr && dt != 0.0f)
	{
		firerate.counter += dt;
		if (firerate.counter >= firerate.iterations)
		{
			Attack();
			firerate.counter = 0;
		}
	}

	FindTarget();

	App->render->AddBlitEvent(1, texture, position.x, position.y, rect, false, 0.0f, false);
	//App->render->AddBlitEvent(1, nullptr, 0, 0, rect, false, false, color.r, color.g, color.b, color.a);
	//App->render->AddBlitEvent(1, animation.texture, 0, 0, {(int)position.x, (int)position.y, animation.GetCurrentFrame().w, animation.GetCurrentFrame().h});
	if (health == 0)
		CleanUp();
}

void Boat::CleanUp()
{
	to_delete = true;
}

void  Boat::Move(float dt)
{
	orientation = Orientation::NONE;

	if (position.x < destination.x)
	{
		position.x += speed * dt;
		if (position.x > destination.x)
			position.x = destination.x;

		orientation = Orientation::EAST;
	}
	if (position.x > destination.x)
	{
		position.x -= speed * dt;
		if (position.x < destination.x)
			position.x = destination.x;

		orientation = Orientation::WEST;
	}
	if (position.y < destination.y)
	{
		position.y += speed * dt;
		if (position.y > destination.y)
			position.y = destination.y;

		if(orientation == Orientation::NONE)
			orientation = Orientation::SOUTH;
		else if (orientation == Orientation::EAST)
			orientation = Orientation::SOUTH_EAST;
		else if (orientation == Orientation::WEST)
			orientation = Orientation::SOUTH_WEST;
	}
	if (position.y > destination.y)
	{
		position.y -= speed * dt;
		if (position.y < destination.y)
			position.y = destination.y;

		if (orientation == Orientation::NONE)
			orientation = Orientation::NORTH;
		else if (orientation == Orientation::EAST)
			orientation = Orientation::NORTH_EAST;
		else if (orientation == Orientation::WEST)
			orientation = Orientation::NORTH_WEST;
	}
	
}

void  Boat::SetDestination()
{
	int mx, my;
	App->input->GetMousePosition(mx, my);
	destination.x = mx - App->render->camera.x / App->win->GetScale();
	destination.y = my - App->render->camera.y / App->win->GetScale();	
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
