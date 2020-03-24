#include "j1Entities.h"

#include "j1Render.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include <vector>
#include "j1Map.h"
#include "j1ParticleManager.h"

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
	if (dt != 0.0f)
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
		else
		{
			NextStep();

			if (target != nullptr)
			{
				firerate.counter += dt;
				if (firerate.counter >= firerate.iterations)
				{
					Damage(10, target);
					firerate.counter = 0;
				}
			}
		}

		target = FindTarget(range, EntityType::NONE);

	}

	App->render->AddBlitEvent(1, texture, GetRenderPositionX(), GetRenderPositionY(), rect);

	if (health == 0)
		CleanUp();
}

void j1Boat::CleanUp()
{
	path.erase(path.begin(), path.end());
	path.shrink_to_fit();

	tradeable_list.erase(tradeable_list.begin(), tradeable_list.end());
	tradeable_list.shrink_to_fit();

	to_delete = true;
}

void  j1Boat::Move(float dt)
{
	orientation = Orientation::NONE;

	if (position.x < destination.x)
	{
		orientation = Orientation::EAST;
		position.x += speed * dt;

		if (position.x > destination.x)
			position.x = destination.x;
	}
	if (position.x > destination.x)
	{
		orientation = Orientation::WEST;
		position.x -= speed * dt;

		if (position.x < destination.x)
			position.x = destination.x;
	}
	if (position.y < destination.y)
	{
		if (orientation == Orientation::NONE)
		{
			orientation = Orientation::SOUTH;
			position.y += speed * dt;
		}
		else
		{
			if (orientation == Orientation::EAST)
				orientation = Orientation::SOUTH_EAST;
			else// if (orientation == Orientation::WEST)
				orientation = Orientation::SOUTH_WEST;

			position.y += speed / 2 * dt;
		}

		if (position.y > destination.y)
			position.y = destination.y;
	}
	if (position.y > destination.y)
	{
		if (orientation == Orientation::NONE)
		{
			orientation = Orientation::NORTH;
			position.y -= speed * dt;
		}
		else
		{
			if (orientation == Orientation::EAST)
				orientation = Orientation::NORTH_EAST;
			else// if (orientation == Orientation::WEST)
				orientation = Orientation::NORTH_WEST;

			position.y -= speed / 2 * dt;
		}

		if (position.y < destination.y)
			position.y = destination.y;
	}	
}

void j1Boat::NextStep()
{
	if (path.size() != 0)
	{
		path.erase(path.begin());
		destination = *path.begin();
	}
}

void  j1Boat::SetDestination()
{
	iPoint m;
	App->input->GetMousePosition(m.x, m.y);
	m.x -= App->render->camera.x / App->win->GetScale();
	m.y -= App->render->camera.y / App->win->GetScale();
	GoTo({ (float)m.x, (float)m.y }, WATER);
}

void j1Boat::Damage(int damage, j1Entity* target)
{
	if (target->health != 0.0f)
	{
		target->health -= damage;
		if (target->health < 0)
			target->health = 0;
		//App->pmanager->createSystem(PARTICLE_TYPES::FIRE, target->position, 1);
	}
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
