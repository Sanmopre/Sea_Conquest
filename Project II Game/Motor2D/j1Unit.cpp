#include "j1Entities.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1InGameUI.h"
#include "j1EntityManager.h"

j1Unit::j1Unit() 
{ 
	main_type = EntityType::UNIT;
	orientation = Orientation::NORTH_WEST; 
	player_command = false;
}

j1Unit::~j1Unit()
{
	target = nullptr;
	path.erase(path.begin(), path.end());
	path.shrink_to_fit();
}

void j1Unit::Primitive_Update(float dt)
{
	showing_hpbar = false;

	SelectAnimation();

	if (App->InGameUI->selected != nullptr)
		if (this == App->InGameUI->selected->trading_entity)
			ShowHPbar(10, 5, -10);

	if (selected)
	{
		if (this == App->InGameUI->selected)
		{
			Trading();
			App->render->AddBlitEvent(0, nullptr, 1, 0, { (int)position.x,(int)position.y + 16, trading_range, 0 }, false, false, 0, 255, 0, 200);
		}

		ShowHPbar(10, 5, -10);

		App->render->AddBlitEvent(0, nullptr, 1, 0, { (int)position.x,(int)position.y + 16, range, 0 }, false, false, 255, 0, 0);
	}

	selectable_area.x = GetRenderPositionX();
	selectable_area.y = GetRenderPositionY();
}

void j1Unit::UpdateMap(Node* node)
{
	map = &node->island->map;
}

void j1Unit::GoTo(fPoint destination, NodeType terrain)
{
	path.swap(App->pathfinding->PathTo(this->position, destination, terrain, map));

	if(path.size() != 0)
		this->destination = *path.begin();

	if (team == 1)
		player_command = true;
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
		if (path.size() == 1)
			player_command = false;
		path.erase(path.begin());
		destination = *path.begin();
	}
}

void  j1Unit::SetPosition(fPoint position)
{
	this->position = position;
	destination = position;
	path.erase(path.begin(), path.end());
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

void j1Unit::Chase(int range, int enemy)
{
	if (target == nullptr)
	{
		if (path.size() == 0)
		{
			j1Entity* chased = FindTarget(position.x, position.y, range, EntityType::NONE, EntityType::NONE, enemy);
			if (chased != nullptr)
			{
				GoTo(chased->position, terrain);
				player_command = false;
			}
		}
	}
	else
	{
		if (path.size() != 0 && !player_command)
		{
			destination = *path.begin();
			path.erase(path.begin(), path.end());
		}
	}

	if (selected && App->show_chasing_range)
		App->render->AddBlitEvent(0, nullptr, 1, 0, { (int)position.x,(int)position.y + 16, range, 0 }, false, false, 0, 0, 255);
}

void j1Unit::GetBasicAnimations()
{
	north = App->anim->GetAnimation("unit-north");
	north_east = App->anim->GetAnimation("unit-north_east");
	north_west = App->anim->GetAnimation("unit-north_west");
	south = App->anim->GetAnimation("unit-south");
	south_east = App->anim->GetAnimation("unit-south_east");
	south_west = App->anim->GetAnimation("unit-south_west");
	east = App->anim->GetAnimation("unit-east");
	west = App->anim->GetAnimation("unit-west");

	rect = north.GetCurrentFrame();
}