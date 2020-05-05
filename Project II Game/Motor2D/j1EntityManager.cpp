#include "j1EntityManager.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "j1App.h"

#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"
#include <algorithm>

using namespace std;

struct {
	bool operator()(j1Entity* a, j1Entity* b) const
	{
		return a->position.y < b->position.y;
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
#include "j1Map.h"
#include "j1Pathfinding.h"

bool j1EntityManager::Update(float dt)
{
	int n = 0;
	int counter = 0;
	while (counter != entities.size())
	{
		vector<j1Entity*>::iterator entity = entities.begin();
		entity += counter;
		for (; entity != entities.end(); entity++)
		{
			if (!(*entity)->to_delete)
			{
				(*entity)->Primitive_Update(dt);
				(*entity)->Update(dt);
				if ((*entity)->selected)
					n++;
			}
			else
			{
				QuickDeleteEntity(entity);
				break;
			}

			counter++;
		}
	}
	selected_n = n;
	if (buffer.size() != 0)
	{
		for (vector<j1Entity*>::iterator bufferentity = buffer.begin(); bufferentity != buffer.end(); bufferentity++)
		{
			entities.push_back(*bufferentity);
		}
		buffer.erase(buffer.begin(), buffer.end());
		buffer.shrink_to_fit();
	}
	std::sort(entities.begin(), entities.end(), customLess);
	////////////////////////////////////ENTITIES_DEBUG///////////////////////////////////////////////////
	if (App->godmode)
	{
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			iPoint test;
			App->input->GetMousePosition(test.x, test.y);
			test.x -= App->render->camera.x / App->win->GetScale();
			test.y -= App->render->camera.y / App->win->GetScale();

			iPoint tile = App->map->WorldToMap(test.x, test.y);
			NodeType terrain = NodeType::WATER;

			SpreadState state = UP;
			int limit = 7;
			int i = 1;
			int x;
			int y;
			int loop = 0;

			while (i <= limit)
			{
				switch (state)
				{
				case UP:
					x = tile.x + loop;
					y = tile.y - i / 2;
					if (i != 1)
						state = RIGHT;
					else
						i += 2;
					break;
				case RIGHT:
					x = tile.x + i / 2;
					y = tile.y + loop;
					state = DOWN;
					break;
				case DOWN:
					x = tile.x - loop;
					y = tile.y + i / 2;
					state = LEFT;
					break;
				case LEFT:
					x = tile.x - i / 2;
					y = tile.y - loop;
					state = UP;
					if (loop > 0)
						loop *= -1;
					else
					{
						loop *= -1;
						loop++;
					}
					if (tile.y - i / 2 == y)
					{
						loop = 0;
						i += 2;
					}
					break;
				}
				bool can = false;
				fPoint pos = {};
				if ((*App->pathfinding->PointToNode(x, y, App->pathfinding->NodeMap))->type == terrain)
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
					AddEntity(pos.x, pos.y, EntityType::BOAT, 3, 1);
					break;
				}
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		{
			iPoint test;
			App->input->GetMousePosition(test.x, test.y);
			test.x -= App->render->camera.x / App->win->GetScale();
			test.y -= App->render->camera.y / App->win->GetScale();
			AddEntity(test.x, test.y, EntityType::BOAT, 1, 2);
		}

		if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
		{	
			iPoint test;
			App->input->GetMousePosition(test.x, test.y);
			test.x -= App->render->camera.x / App->win->GetScale();
			test.y -= App->render->camera.y / App->win->GetScale();

			AddEntity(test.x, test.y, EntityType::BOATHOUSE, 0, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		{
			iPoint test;
			App->input->GetMousePosition(test.x, test.y);
			test.x -= App->render->camera.x / App->win->GetScale();
			test.y -= App->render->camera.y / App->win->GetScale();

			AddEntity(test.x, test.y, EntityType::BOATHOUSE, 0, 2);
		}

		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			iPoint test;
			App->input->GetMousePosition(test.x, test.y);
			test.x -= App->render->camera.x / App->win->GetScale();
			test.y -= App->render->camera.y / App->win->GetScale();
			AddEntity(test.x, test.y, EntityType::HARVESTER, 1, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			iPoint test;
			App->input->GetMousePosition(test.x, test.y);
			test.x -= App->render->camera.x / App->win->GetScale();
			test.y -= App->render->camera.y / App->win->GetScale();

			AddEntity(test.x, test.y, EntityType::STORAGE, 0, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			iPoint test;
			App->input->GetMousePosition(test.x, test.y);
			test.x -= App->render->camera.x / App->win->GetScale();
			test.y -= App->render->camera.y / App->win->GetScale();
			AddEntity(test.x, test.y, EntityType::ALL_COTTON, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
		{
			iPoint test;
			App->input->GetMousePosition(test.x, test.y);
			test.x -= App->render->camera.x / App->win->GetScale();
			test.y -= App->render->camera.y / App->win->GetScale();
			AddEntity(test.x, test.y, EntityType::ALL_WOOD, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN)
		{
			iPoint test;
			App->input->GetMousePosition(test.x, test.y);
			test.x -= App->render->camera.x / App->win->GetScale();
			test.y -= App->render->camera.y / App->win->GetScale();
			AddEntity(test.x, test.y, EntityType::ALL_METAL, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		{
			for (vector<j1Entity*>::iterator entity = entities.begin(); entity != entities.end(); entity++)
				if ((*entity)->selected)
					(*entity)->CleanUp();
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
//Types: BOAT, HARVESTER, BOATHOUSE, STORAGE, STORAGE, TOWNHALL, ALL_COTTON, ALL_WOOD, ALL_METAL
j1Entity* j1EntityManager::AddEntity(float x, float y, EntityType type, int level, int team)
{
	switch (type)
	{
	case EntityType::BOAT:
		buffer.push_back(new j1Boat(x, y, level, team));
		break;
	case EntityType::HARVESTER:
		buffer.push_back(new j1Harvester(x, y, level, team));
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

void j1EntityManager::DeleteEntity(j1Entity* entity_)
{
	if(entities.size() != 0)
		for (auto entity = entities.begin(); entity != entities.end(); entity++)
		{
			if ((*entity) == entity_)
			{
				(*entity)->CleanUp();
				delete (*entity);
				entities.erase(entity, entity + 1);
				if(entities.size() <= entities.capacity()/2)
					entities.shrink_to_fit();
				break;
			}
		}
}

void j1EntityManager::QuickDeleteEntity(std::vector<j1Entity*>::iterator entity)
{
	delete (*entity);
	entities.erase(entity, entity + 1);
	if (entities.size() <= entities.capacity() / 2)
		entities.shrink_to_fit();
}

void j1EntityManager::DeleteAll()
{
	while (entities.size() != 0)
	{
		(*entities.begin())->CleanUp(); 
		delete (*entities.begin());
		entities.erase(entities.begin(), entities.begin() + 1);
	}
	entities.shrink_to_fit();

	while (buffer.size() != 0)
	{
		(*entities.begin())->CleanUp();  
		delete (*buffer.begin());
		buffer.erase(buffer.begin(), buffer.begin() + 1);
	}
	buffer.shrink_to_fit();
}

bool j1EntityManager::CleanUp()
{
	DeleteAll();

	return true;
}