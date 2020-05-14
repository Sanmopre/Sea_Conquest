#include "j1Entities.h"

#include "j1EntityManager.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Minimap.h"

j1Carrier::j1Carrier(float x, float y, int level, int team)
{
	type = EntityType::CARRIER;
	terrain = NodeType::WATER;
	fog_range = 4;

	iPoint tile = App->map->WorldToMap(x, y);
	position = App->map->MapToWorld<fPoint>(tile.x, tile.y);
	destination = position;

	this->level = level;
	trading_range = 50;
	this->team = team;
	speed = 40;
	range = 60;

	max_health = 100;
	health = max_health;
	load = { 0, 0, 0, 0 };
	target = nullptr;

	texture = App->tex->GetTexture("carrier", level, team);
	shadow = App->tex->GetTexture("carrier-shadow", 0, 0);

	GetBasicAnimations();
	selectable_area = rect;
}

j1Carrier::~j1Carrier()
{
	CleanUp();
}

void j1Carrier::Update(float dt)
{
	if (dt != 0.0f)
	{
		if (selected)
		{
			if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
				Store();
			if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
				if (units.size() != 0)
					Deploy();
		}

		if (destination != position)
			Move(dt);
		else
		{
			NextStep();
		}
	}

	if (App->fog->GetVisibility(position) == FogState::VISIBLE || App->ignore_fog)
	{
		App->render->AddBlitEvent(1, shadow, GetRenderPositionX(), GetRenderPositionY(), rect, false, false, 0, 0, 0, 100);
		App->render->AddBlitEvent(1, texture, GetRenderPositionX(), GetRenderPositionY(), rect);
		App->minimap->Draw_entities(this);
	}

	if (health <= 0)
	{
		CleanUp();
	}
}

void j1Carrier::CleanUp()
{
	to_delete = true;

	while (units.size() != 0)
	{
		delete *units.begin();
		units.erase(units.begin());
	}
	units.shrink_to_fit();
}

void j1Carrier::Store()
{
	if(trading_entity != nullptr)
		if (trading_entity->main_type == EntityType::UNIT && trading_entity->terrain == NodeType::GROUND)
		{
			units.push_back(trading_entity);
			trading_entity->to_remove = true;
			trading_entity = nullptr;
		}
}

void j1Carrier::Deploy()
{
	iPoint tile = App->map->WorldToMap(position.x, position.y);
	int x = tile.x;
	int y = tile.y;
	j1Entity* entity = *units.begin();

	int i = 3;
	SpreadState state = UP;
	int limit = 5;
	while (i <= limit)
	{
		switch (state)
		{
		case UP:
			y -= i / 2;
			if (i != 1)
				state = RIGHT;
			else
				i += 2;
			break;
		case RIGHT:
			x++;
			if (x == tile.x + i / 2)
				state = DOWN;
			break;
		case DOWN:
			y++;
			if (y == tile.y + i / 2)
				state = LEFT;
			break;
		case LEFT:
			x--;
			if (x == tile.x - i / 2)
				state = UP2;
			break;
		case UP2:
			y--;
			if (y == tile.y - i / 2)
				state = RIGHT2;
			break;
		case RIGHT2:
			if (x == tile.x - 1)
			{
				i += 2;
				x = tile.x;
				y = tile.y;
				state = UP;
			}
			else
				x++;
			break;
		}
		bool can = false;
		fPoint pos = {};
		Node* node = *App->pathfinding->WorldToNode(x, y);
		if (node->type == entity->terrain)
		{
			can = true;
			pos = App->map->MapToWorld<fPoint>(x, y);
			for (vector<j1Entity*>::iterator e = App->entitymanager->entities.begin(); e != App->entitymanager->entities.end(); e++)
			{
				if ((*e)->main_type == EntityType::UNIT)
					if ((*e)->position == pos)
					{
						can = false;
						break;
					}
			}
		}
		if (can)
		{
			entity->SetPosition(pos);
			entity->UpdateMap(node);
			entity->to_remove = false;

			App->entitymanager->AddToBuffer(entity);
			units.erase(units.begin());
			break;
		}
	}
}