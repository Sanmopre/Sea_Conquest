#include "j1Entities.h"

#include "j1Render.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include <vector>
#include "j1Map.h"

j1Boat::j1Boat(float x, float y, int level, int team)
{
	type = EntityType::BOAT;
	position.x = x;
	position.y = y;
	destination = position;
	this->level = level;
	trading_range = 50;
	this->team = team;
	speed = 50;
	range = 100;
	firerate = { 1 };
	max_health = 100;
	health = max_health;
	storage = { 0, 0, 0, 200 };
	
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

j1Boat::~j1Boat()
{

}

void j1Boat::Update(float dt)
{
	showing_hpbar = false;

	SelectAnimation();
	
	if (selected)
	{
		if (team == 0)
		{
			if (App->input->GetMouseButtonDown(3) == KEY_DOWN)
				SetDestination();

			Trading();
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

	App->render->AddBlitEvent(1, texture, position.x, position.y, rect);
	
	if (health == 0)
		CleanUp();
}

void j1Boat::CleanUp()
{
	to_delete = true;
}

void  j1Boat::Move(float dt)
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

void  j1Boat::SetDestination()
{
	int mx, my;
	App->input->GetMousePosition(mx, my);
	destination.x = mx - App->render->camera.x / App->win->GetScale();
	destination.y = my - App->render->camera.y / App->win->GetScale();	
}

void  j1Boat::Attack()
{
	Damage(10, target);
}

void  j1Boat::FindTarget()
{
	float targetdistance = range;
	float distance = 0.0f;

	for (std::vector<j1Entity*>::iterator e = App->entitymanager->entities.begin(); e != App->entitymanager->entities.end(); e++)
	{
		if (*e != this && (*e)->team != team)
			if (position.x + range > (*e)->position.x &&
				position.x - range < (*e)->position.x &&
				position.y + range >(*e)->position.y &&
				position.y - range < (*e)->position.y)
			{
				distance = sqrtf((position.x - (*e)->position.x) * (position.x - (*e)->position.x) + (position.y - (*e)->position.y) * (position.y - (*e)->position.y));

				if (distance < range && distance < targetdistance)
				{
					target = *e;
					targetdistance = distance;
				}
				
				ShowHPbar(10, 5);
			}
	}

	if(distance == 0.0f)
		target = nullptr;
}

void j1Boat::SelectAnimation()
{
	switch (orientation)
	{
	case Orientation::NORTH:
		rect = north.GetCurrentFrame();
		texture = north.texture;
		break;

	case Orientation::NORTH_EAST:
		rect = north_east.GetCurrentFrame();

		break;

	case Orientation::EAST:
		rect = east.GetCurrentFrame();

		break;

	case Orientation::SOUTH_EAST:
		rect = south_east.GetCurrentFrame();

		break;

	case Orientation::SOUTH:
		rect = south.GetCurrentFrame();

		break;

	case Orientation::SOUTH_WEST:
		rect = south_west.GetCurrentFrame();

		break;

	case Orientation::WEST:
		rect = west.GetCurrentFrame();

		break;

	case Orientation::NORTH_WEST:
		rect = north_west.GetCurrentFrame();

		break;
	}
}
