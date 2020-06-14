#include "j1EntityManager.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "j1App.h"

#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "j1Fog.h"
#include <algorithm>

using namespace std;

struct {
	bool operator()(j1Entity* a, j1Entity* b) const
	{
		if(a->position.y != b->position.y)
			return a->position.y < b->position.y;
		else if (a->position.x != b->position.x)
			return a->position.x < b->position.x;
		else
		{
			if(a->terrain == NodeType::ALL)
				return false;
			else if (b->terrain == NodeType::ALL)
				return true;
		}
		return false;
	}
} customLess;


j1EntityManager::j1EntityManager()
{

}

j1EntityManager::~j1EntityManager()
{

}

bool j1EntityManager::Start()
{
	LOG("EntityManager Started");
	return true;
}
#include "j1Fonts.h"
bool j1EntityManager::Update(float dt)
{
	int counter = 0;

	ally_entities.erase(ally_entities.begin(), ally_entities.end());
	selected_units.erase(selected_units.begin(), selected_units.end());
	air_units.erase(air_units.begin(), air_units.end());

	for (vector<j1Entity*>::iterator e = entities.begin(); e != entities.end(); e++)
	{
		j1Entity* entity = *e; 

		if (entity->health == 0 || entity->to_delete || entity->to_remove)
		{
			for_deletion.push_back(entity);
		}
		else
		{
			iPoint tile = App->map->WorldToMap(entity->position.x, entity->position.y);
			FogState visibility = App->fog->GetVisibility(tile.x, tile.y);

			if (visibility != FogState::FOGGED || entity->team == 1)
			{
				entity->spot = e;
				entity->Primitive_Update(dt);
				entity->Update(dt);

				if (entity->team == 1)
					ally_entities.push_back(entity);
				if (entity->main_type == EntityType::UNIT && entity->selected && entity->team == 1)
					selected_units.push_back(entity);
				if (entity->main_type == EntityType::UNIT && entity->terrain == NodeType::ALL && entity->team == 1)
					air_units.push_back(entity);
			}
		}
		
		counter++;
		if (App->numerate_entities)
		{
			char text[10];
			sprintf_s(text, 10, "%7d", counter);
			App->fonts->BlitText(entity->position.x - 120 + App->render->camera.x, entity->position.y + App->render->camera.y, 1, text);
		}
	}
	
	for (vector<j1Entity*>::iterator deleted_entity = for_deletion.begin(); deleted_entity != for_deletion.end(); deleted_entity++)
		DeleteEntity(*deleted_entity);
	for_deletion.erase(for_deletion.begin(), for_deletion.end());
	
	for (vector<j1Entity*>::iterator bufferentity = buffer.begin(); bufferentity != buffer.end(); bufferentity++)
	{
		entities.push_back(*bufferentity);
	}
	buffer.erase(buffer.begin(), buffer.end());
	buffer.shrink_to_fit();

	std::sort(entities.begin(), entities.end(), customLess);
	////////////////////////////////////ENTITIES_DEBUG///////////////////////////////////////////////////
	if (App->godmode)
	{

		iPoint test;
		App->input->GetMousePosition(test.x, test.y);
		test.x -= App->render->camera.x / App->win->GetScale();
		test.y -= App->render->camera.y / App->win->GetScale();
		if (App->map->mapdata != nullptr)
		{
			test = App->map->WorldToMap(test.x, test.y);
			test = App->map->MapToWorld<iPoint>(test.x, test.y);
		}

		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
		{
			AddEntity(test.x, test.y, EntityType::CARRIER, 1, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			AddEntity(test.x, test.y, EntityType::BOAT, 2, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		{
			AddEntity(test.x, test.y, EntityType::BOAT, 2, 2);
		}

		if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
		{	
			AddEntity(test.x, test.y, EntityType::BOATHOUSE, 0, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		{
			AddEntity(test.x, test.y, EntityType::BOATHOUSE, 0, 2);
		}

		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			AddEntity(test.x, test.y, EntityType::HARVESTER, 2, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{

			AddEntity(test.x, test.y, EntityType::STORAGE, 0, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			AddEntity(test.x, test.y, EntityType::ALL_COTTON, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
		{
			AddEntity(test.x, test.y, EntityType::ALL_WOOD, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN)
		{
			AddEntity(test.x, test.y, EntityType::ALL_METAL, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		{
			for (vector<j1Entity*>::iterator entity = entities.begin(); entity != entities.end(); entity++)
				if ((*entity)->selected)
					(*entity)->to_delete = true;
		}
		if (App->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN)
		{
			AddEntity(test.x, test.y, EntityType::BALLOON, 1, 1);
		}
		if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
		{
			AddEntity(test.x, test.y, EntityType::BALLOON, 2, 1);
		}
		if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
		{
			for (vector<j1Entity*>::iterator entity = entities.begin(); entity != entities.end(); entity++)
				if ((*entity)->selected)
				{
					int d;
					int h;
					int m;
					int s;
					int r;

					(*entity)->GetStats(d, h, m, s, r);
					LOG("Damage: %d, Health: %d, MaxHealth: %d, Speed: %d, MaxResources: %d", d, h, m, s, r);
				}
		}
		if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
		{
			DeleteAll();
		}
	}
	//*///////////////////////////////////////////////////////////////////////////////////////////////////
	return true;
}

//Schedules an entity creation in the next iteration, however the entity can be modified in the iteration of cretion with out problem. 
//Types: BOAT, BALOON, HARVESTER, CARRIER, BOATHOUSE, STORAGE, STORAGE, TOWNHALL, TURRET, ALL_COTTON, ALL_WOOD, ALL_METAL
j1Entity* j1EntityManager::AddEntity(float x, float y, EntityType type, int level, int team)
{
	iPoint tile = App->map->WorldToMap(x, y);
	if (tile.x >= 0 && tile.x < App->map->mapdata->width && tile.y >= 0 && tile.y < App->map->mapdata->height)
	{
		switch (type)
		{
		case EntityType::BOAT:
			buffer.push_back(new j1Boat(x, y, level, team));
			break;
		case EntityType::BALLOON:
			buffer.push_back(new j1Balloon(x, y, level, team));
			break;
		case EntityType::HARVESTER:
			buffer.push_back(new j1Harvester(x, y, level, team));
			break;
		case EntityType::CARRIER:
			buffer.push_back(new j1Carrier(x, y, level, team));
			break;
		case EntityType::BOATHOUSE:
			buffer.push_back(new j1BoatHouse(x, y, team));
			break;
		case EntityType::STORAGE:
			buffer.push_back(new j1Storage(x, y, team));
			break;
		case EntityType::TOWNHALL:
			buffer.push_back(new j1TownHall(x, y, team));
			break;
		case EntityType::TURRET:
			buffer.push_back(new Turret(x, y, team));
			break;
		case EntityType::ALL_COTTON:
			buffer.push_back(new j1Resource(x, y, level, type));
			break;
		case EntityType::ALL_WOOD:
			buffer.push_back(new j1Resource(x, y, level, type));
			break;
		case EntityType::ALL_METAL:
			buffer.push_back(new j1Resource(x, y, level, type));
			break;
		}

		return *(buffer.end() - 1);
	}

	return nullptr;
}

void j1EntityManager::AddToBuffer(j1Entity* entity)
{
	buffer.push_back(entity);
}

void j1EntityManager::DeleteEntity(j1Entity* entity_)
{
	if(entities.size() != 0)
		for (auto entity = entities.begin(); entity != entities.end(); entity++)
		{
			if (*entity == entity_)
			{
				if (!(*entity)->to_remove)
				{
					(*entity)->CleanUp();
					delete (*entity);
				}
				entities.erase(entity);
				break;
			}
		}
}

void j1EntityManager::QuickDeleteEntity(std::vector<j1Entity*>::iterator entity)
{
	if (entity < entities.begin())
		entity = entities.begin();
	else if (entity >= entities.end())
		entity = entities.end() - 1;

	if((*entity)->to_delete)
		delete (*entity);
	entities.erase(entity);
}

void j1EntityManager::DeleteAll()
{
	while (entities.size() != 0)
	{
		(*entities.begin())->CleanUp(); 
		delete (*entities.begin());
		entities.erase(entities.begin(), entities.begin() + 1);
	}

	while (buffer.size() != 0)
	{
		(*entities.begin())->CleanUp();  
		delete (*buffer.begin());
		buffer.erase(buffer.begin(), buffer.begin() + 1);
	}
}

bool j1EntityManager::CleanUp()
{
	townhall_level = 0;
	DeleteAll();

	vector<j1Entity*> e;
	entities.swap(e);
	vector<j1Entity*> a;
	ally_entities.swap(a);
	vector<j1Entity*> s;
	selected_units.swap(s);
	vector<j1Entity*> i;
	air_units.swap(i);
	vector<j1Entity*> b;
	buffer.swap(b);
	vector<j1Entity*> f;
	for_deletion.swap(f);

	return true;
}

bool j1EntityManager::InsideElipse(fPoint center, fPoint point, int range)
{
	float p = (pow((point.x - center.x), 2) / pow(range, 2))
		+ (pow((point.y - center.y), 2) / pow(range/2, 2));

	if (p > 1)
		return false;
	else if (p <= 1)
		return true;
}

bool j1EntityManager::Load(pugi::xml_node& data)
{
	pugi::xml_node node = data.child("entity_manager").child("entity");

	for (node; node != NULL; node = node.next_sibling("entity"))
	{
		pugi::xml_node create = node.child("create");

		float x = create.attribute("x").as_float();
		float y = create.attribute("y").as_float();
		EntityType type = (EntityType)create.attribute("type").as_int();
		int level = create.attribute("level").as_int();
		int team = create.attribute("team").as_int();

		j1Entity* entity = AddEntity(x, y, type, level, team);
		
		pugi::xml_node restore = node.child("restore");
		
		entity->to_delete = restore.attribute("to_delete").as_bool();
		entity->to_remove = restore.attribute("to_remove").as_bool();
		entity->health = restore.attribute("health").as_float();
		entity->selected = restore.attribute("selected").as_bool();
		
		pugi::xml_node load = restore.child("load");
		entity->load.cotton = load.attribute("cotton").as_int();
		entity->load.wood = load.attribute("wood").as_int();
		entity->load.metal = load.attribute("metal").as_int();
		entity->load.maxweight = load.attribute("maxweight").as_int();

		entity->InfoLoad(restore);
	}

	return true;
}

bool j1EntityManager::Save(pugi::xml_node& data) const
{
	pugi::xml_node node = data.append_child("entity_manager");

	for (auto e = entities.begin(); e != entities.end(); e++)
	{
		j1Entity* entity = *e;

		pugi::xml_node ent_node = node.append_child("entity");

		pugi::xml_node create = ent_node.append_child("create");
		create.append_attribute("x").set_value(entity->position.x);
		create.append_attribute("y").set_value(entity->position.y);
		create.append_attribute("type").set_value((int)entity->type);
		create.append_attribute("level").set_value(entity->level);
		create.append_attribute("team").set_value(entity->team);

		pugi::xml_node restore = ent_node.append_child("restore");
		restore.append_attribute("to_delete").set_value(entity->to_delete);
		restore.append_attribute("to_remove").set_value(entity->to_remove);
		restore.append_attribute("health").set_value(entity->health);
		restore.append_attribute("selected").set_value(entity->selected);

		pugi::xml_node load = restore.append_child("load");
		load.append_attribute("cotton").set_value(entity->load.cotton);
		load.append_attribute("wood").set_value(entity->load.wood);
		load.append_attribute("metal").set_value(entity->load.metal);
		load.append_attribute("maxweight").set_value(entity->load.maxweight);

		entity->Save(restore);
	}

	return true;
}