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

	this->team = team;

	int extra = level * 1.5;
	trading_range = 50 + 13 * extra;
	speed = 30 + 10 * extra;
	range = 60;

	stored_units = 0;
	capacity = level;

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
	if(units.size() < capacity)
	if(trading_entity != nullptr)
		if (trading_entity->main_type == EntityType::UNIT && trading_entity->terrain == NodeType::GROUND)
		{
			units.push_back(trading_entity);
			trading_entity->to_remove = true;
			trading_entity = nullptr;
			stored_units++;
		}
}

void j1Carrier::Deploy()
{
	if (units.size() != 0)
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
				stored_units--;
				break;
			}
		}
	}
}

void j1Carrier::GetUnitsInfo(int& harvesters, int& tanks, int& capacity)
{
	int h = 0;
	int t = 0;
	for (vector<j1Entity*>::iterator itr = units.begin(); itr != units.end(); itr++)
	{
		j1Entity* entity = *itr;
		if (entity->type == EntityType::HARVESTER)
			h++;
		else if (entity->type == EntityType::TANK)
			t++;
	}

	harvesters = h;
	tanks = t;
	capacity = this->capacity;
}

void j1Carrier::DeepInfoLoad(pugi::xml_node& data)
{
	//<deep stored_units = "1" capacity = "1">
	//	<units>
	//		<entity>
	//			<create type = "3" level = "1" team = "1" / >
	//			<restore health = "60">
	//				<load cotton = "0" wood = "90" metal = "0" maxweight = "600" / >
	pugi::xml_node deep = data.child("deep");

	stored_units = deep.attribute("stored_units").as_int();
	capacity = deep.attribute("capacity").as_int();

	pugi::xml_node entity = deep.child("units").child("entity");

	for (entity; entity != NULL; entity = entity.next_sibling("entity"))
	{
		pugi::xml_node create = entity.child("create");

		float x = 0;
		float y = 0;
		EntityType type = (EntityType)create.attribute("type").as_int();
		int level = create.attribute("level").as_int();
		int team = create.attribute("team").as_int();

		j1Entity* ent = App->entitymanager->AddEntity(x, y, type, level, team);

		pugi::xml_node restore = entity.child("restore");

		ent->to_delete = false;
		ent->to_remove = true;
		ent->health = restore.attribute("health").as_float();

		pugi::xml_node load = restore.child("load");
		ent->load.cotton = load.attribute("cotton").as_int();
		ent->load.wood = load.attribute("wood").as_int();
		ent->load.metal = load.attribute("metal").as_int();
		ent->load.maxweight = load.attribute("maxweight").as_int();

		units.push_back(ent);
	}
}

void j1Carrier::DeepSave(pugi::xml_node& data)
{
	pugi::xml_node node = data.append_child("deep");

	node.append_attribute("stored_units").set_value(stored_units);
	node.append_attribute("capacity").set_value(capacity);

	node = node.append_child("units");
	for (auto e = units.begin(); e != units.end(); e++)
	{
		j1Entity* entity = *e;

		pugi::xml_node ent_node = node.append_child("entity");

		pugi::xml_node create = ent_node.append_child("create");
		create.append_attribute("type").set_value((int)entity->type);
		create.append_attribute("level").set_value(entity->level);
		create.append_attribute("team").set_value(entity->team);

		pugi::xml_node restore = ent_node.append_child("restore");
		restore.append_attribute("health").set_value(entity->health);

		pugi::xml_node load = restore.append_child("load");
		load.append_attribute("cotton").set_value(entity->load.cotton);
		load.append_attribute("wood").set_value(entity->load.wood);
		load.append_attribute("metal").set_value(entity->load.metal);
		load.append_attribute("maxweight").set_value(entity->load.maxweight);
	}
}