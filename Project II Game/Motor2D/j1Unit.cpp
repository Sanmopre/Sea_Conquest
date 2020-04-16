#include "j1Entities.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"

j1Unit::j1Unit() 
{ 
	main_type = EntityType::UNIT;
	orientation = Orientation::NORTH; 
}

j1Unit::~j1Unit()
{
	path.erase(path.begin(), path.end());
	path.shrink_to_fit();
}

void j1Unit::GoTo(fPoint destination, NodeType terrain)
{
	path.swap(App->pathfinding->PathTo(this->position, destination, terrain));

	if(path.size() != 0)
		this->destination = *path.begin();
}

void  j1Unit::Move(float dt)
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

void j1Unit::NextStep()
{
	if (path.size() != 0)
	{
		path.erase(path.begin());
		destination = *path.begin();
	}
}

void  j1Unit::SetDestination(NodeType terrain)
{
	iPoint m;
	App->input->GetMousePosition(m.x, m.y);
	m.x -= App->render->camera.x / App->win->GetScale();
	m.y -= App->render->camera.y / App->win->GetScale();
	GoTo({ (float)m.x, (float)m.y }, terrain);
}

void j1Unit::SelectAnimation()
{
	switch (orientation)
	{
	case Orientation::NORTH:
		rect = north.GetCurrentFrame();
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

SDL_Texture* j1Entity::LoadTexture(j1Entity* entity, std::vector<TextureInfo>& textureBuffer)
{
	SDL_Texture* ret = nullptr;
	if (entity->main_type == EntityType::STRUCTURE)
	{
		for (std::vector<TextureInfo>::iterator e = textureBuffer.begin(); e != textureBuffer.end(); e++)
		{
			if (entity->main_type == e->type && entity->team == e->team)
			{
				ret = e->texture;
				break;
			}
		}
	}
	else if (entity->main_type == EntityType::UNIT)
	{
		for (std::vector<TextureInfo>::iterator e = textureBuffer.begin(); e != textureBuffer.end(); e++)
		{
			if (entity->type == e->type && entity->level == e->level)
			{
				ret = e->texture;
				break;
			}
		}
	}
	return ret;
}