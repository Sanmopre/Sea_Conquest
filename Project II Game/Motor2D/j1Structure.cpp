#include "j1Entities.h"

#include "j1Map.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "j1InGameUI.h"
#include "j1Scene.h"
#include "j1Pathfinding.h"
#include "j1ParticleManager.h"

j1Structure::j1Structure()
{ 
	main_type = EntityType::STRUCTURE;
	
	placed = true;

	if (App->scene->start || App->godmode)
		built_state = NOT_BUILDING;
	else
	{
		built_state = BUILDING;
		health = 1;
	}
}


j1Structure::~j1Structure()
{
	if (App->entitymanager->townhall_level != 0)
	{
		App->audio->PlaySpatialFx(App->audio->structure_destroy,
			App->audio->GetAngle(App->render->getCameraPosition(), { (int)position.x, (int)position.y }),
			App->audio->GetDistance(App->render->getCameraPosition(), { (int)position.x, (int)position.y }));
	}

	current_animation = nullptr;
	current_tex = nullptr;
	texture = nullptr;
	tex_construction = nullptr;
	if(App->pathfinding->NodeMap.size() != 0)
		(*App->pathfinding->WorldToNode(tile.x, tile.y))->built = false;
	unitqueue.erase(unitqueue.begin(), unitqueue.end());
	unitqueue.shrink_to_fit();
}

void j1Structure::Primitive_Update(float dt) 
{
	showing_hpbar = false;

	if (built_state == BUILDING)
	{
		ShowHPbar(10, 5, -20);
	}
	else
	{
		NotPlacedBehaviour();

		if (App->InGameUI->selected != nullptr)
			if (this == App->InGameUI->selected->trading_entity)
				ShowHPbar(10, 5, -20);

		if (selected)
		{
			ShowHPbar(10, 5, -20);

			if (this == App->InGameUI->selected)
			{
				Trading();
				App->render->AddBlitEvent(0, nullptr, 1, 0, { (int)position.x,(int)position.y + 16, trading_range, 0 }, false, false, 0, 255, 0, 200);
			}

			if (App->godmode)
			{
				if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
					placed = false;
			}

		}

		BuildProcces(dt);
	}

	if (built_state == BUILDING || built_state == ON_HOLD)
	{
		current_animation = &under_construction;
		current_tex = tex_construction;
	}
	else
		current_tex = texture;

	if(current_animation != nullptr)
		rect = current_animation->GetCurrentFrame();

	selectable_area = rect;
	selectable_area.x = GetRenderPositionX();
	selectable_area.y = GetRenderPositionY();
	selectable_area.h /= 2;
	selectable_area.y += selectable_area.h;
}

void j1Structure::NotPlacedBehaviour()
{
	if (!placed)
	{
		position = App->input->GetMouseWorldPosition();

		iPoint placing_tile = App->map->WorldToMap(position.x, position.y);
		position = App->map->MapToWorld<fPoint>(placing_tile.x, placing_tile.y);
		if (App->godmode)
		{
			if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
			{
				CleanUp();
				App->audio->PlaySpatialFx(App->audio->structure_destroy,
					App->audio->GetAngle(App->render->getCameraPosition(), { (int)position.x, (int)position.y }),
					App->audio->GetDistance(App->render->getCameraPosition(), { (int)position.x, (int)position.y }));
			}
		}
	}
}

void j1Structure::BuildUnit(EntityType type, int level)
{
	iPoint tile = App->map->WorldToMap(position.x, position.y);
	int x = tile.x;
	int y = tile.y;

	int i = 3;
	SpreadState state = UP;
	int limit = 7;
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
		if (((*App->pathfinding->WorldToNode(x, y))->type == terrain && (*App->pathfinding->WorldToNode(x, y))->built == false) || type == EntityType::BALLOON)
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
			for (vector<EntityRequest>::iterator e = unitqueue.begin(); e != unitqueue.end(); e++)
			{
				fPoint epos = { e->x, e->y };
				if (epos == pos)
					can = false;
			}
		}
		if (can)
		{
			pos = App->map->MapToWorld<fPoint>(x, y);
			EntityRequest unit(pos.x, pos.y, type, level, team);
			unitqueue.push_back(unit);
			break;
		}	
	}
}

void j1Structure::BuildProcces(float dt)
{
	if (unitqueue.size() != 0)
	{
		int l = App->entitymanager->townhall_level;
		switch (unitqueue.begin()->type)
		{
		case EntityType::BOAT:
			building_time.iterations = 5 * level / 2;
			break;
		case EntityType::BALLOON:
			if (l < 3)
				l = 1;
			else
				l = 2;
			building_time.iterations = 10 * level / 2;
			break;
		case EntityType::HARVESTER:
			if (l < 3)
				l = 1;
			else
				l = 2;
			building_time.iterations = 8 * level / 2;
			break;
		case EntityType::CARRIER:
			building_time.iterations = 15 * level / 2;
			break;
		}
		building_time.counter += dt;

		SDL_Rect Brect = { GetRenderPositionX(), GetRenderPositionY() + rect.h, rect.w, 5 };
		Color Bcolor(96u, 96u, 96u);

		float max_w = Brect.w;
		float width = max_w;

		App->render->AddBlitEvent(2, nullptr, 0, 0, Brect, false, false, Bcolor.r, Bcolor.g, Bcolor.b, Bcolor.a); //health grey bar

		width /= building_time.iterations;
		width *= building_time.counter;
		Brect.w = width;
		Bcolor.SetColor(255u, 100u, 0u);
		App->render->AddBlitEvent(2, nullptr, 0, 0, Brect, false, false, Bcolor.r, Bcolor.g, Bcolor.b, Bcolor.a); //health bar

		if (building_time.counter >= building_time.iterations)
		{
			App->entitymanager->AddEntity(unitqueue.begin()->x, unitqueue.begin()->y, unitqueue.begin()->type, l, unitqueue.begin()->team);
			unitqueue.erase(unitqueue.begin());
			if (unitqueue.size() <= unitqueue.capacity() / 2)
				unitqueue.shrink_to_fit();
			building_time.counter = 0;
		}
	}
}

void j1Structure::GetStats(int& attack, int& health, int& maxhealth, int& speed, int& maxresources)
{
	attack = 0;
	health = this->health;
	maxhealth = max_health;
	speed = 0;
	maxresources = load.maxweight;
}

void j1Structure::Save(pugi::xml_node& data)
{
	pugi::xml_node node = data.append_child("structure");

	node.append_attribute("build_state").set_value(built_state);

	pugi::xml_node timer = node.append_child("timer");
	timer.append_attribute("iterations").set_value(building_time.iterations);
	timer.append_attribute("counter").set_value(building_time.counter);

	node = node.append_child("unitqueue");
	for (auto u = unitqueue.begin(); u != unitqueue.end(); u++)
	{
		pugi::xml_node pos = node.append_child("unit");
		pos.append_attribute("x").set_value(u->x);
		pos.append_attribute("y").set_value(u->y);
		pos.append_attribute("type").set_value((int)u->type);
		pos.append_attribute("level").set_value(u->level);
		pos.append_attribute("team").set_value(u->team);
	}
}