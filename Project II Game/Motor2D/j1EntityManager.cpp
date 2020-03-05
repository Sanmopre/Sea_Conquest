#include "j1EntityManager.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "j1App.h"

#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"

using namespace std;

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

bool j1EntityManager::Update(float dt)
{
	int counter = 0;

	while (counter != entities.size())
	{
		vector<Entity*>::iterator entity = entities.begin();
		entity += counter;
		for (; entity != entities.end(); entity++)
		{
			if ((*entity)->to_delete)
			{
				QuickDeleteEntity(entity);
				break;
			}
			else
				(*entity)->Update(dt);

			counter++;
		}
	}

	if (buffer.size() != 0)
	{
		for (vector<Entity*>::iterator bufferentity = buffer.begin(); bufferentity != buffer.end(); bufferentity++)
		{
			entities.push_back(*bufferentity);
		}
		buffer.erase(buffer.begin(), buffer.end());
		buffer.shrink_to_fit();
	}
	////////////////////////////////////ENTITIES_DEBUG///////////////////////////////////////////////////
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		iPoint test;
		App->input->GetMousePosition(test.x, test.y);
		test.x -= App->render->camera.x / App->win->GetScale();
		test.y -= App->render->camera.y / App->win->GetScale();
		AddEntity(test.x, test.y, Entity_Type::BOAT, 1, 0);

		LOG("There is %d entities with %d vector capacity", entities.size(), entities.capacity());
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		iPoint test;
		App->input->GetMousePosition(test.x, test.y);
		test.x -= App->render->camera.x / App->win->GetScale();
		test.y -= App->render->camera.y / App->win->GetScale();
		AddEntity(test.x, test.y, Entity_Type::BOAT, 1, 1);

		LOG("There is %d entities with %d vector capacity", entities.size(), entities.capacity());
	}

	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		AddEntity(0, 0, Entity_Type::BOATHOUSE, 0, 0);

		LOG("There is %d entities with %d vector capacity", entities.size(), entities.capacity());
	}

	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
	{
		AddEntity(0, 0, Entity_Type::BOATHOUSE, 0, 1);

		LOG("There is %d entities with %d vector capacity", entities.size(), entities.capacity());
	}

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		for (vector<Entity*>::iterator entity = entities.begin(); entity != entities.end(); entity++)
		{
			if ((*entity)->selected)
				(*entity)->CleanUp();
		}

		LOG("There is %d entities with %d vector capacity", entities.size(), entities.capacity());
	}

	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		DeleteAll();
		LOG("There is %d entities with %d vector capacity", entities.size(), entities.capacity());
	}
	//*///////////////////////////////////////////////////////////////////////////////////////////////////
	return true;
}

Entity* j1EntityManager::AddEntity(float x, float y, Entity_Type type, int level, int team)
{
	LOG("AA");
	switch (type)
	{
	case Entity_Type::BOAT:
		buffer.push_back(new Boat(x, y, level, team));
		break;
	case Entity_Type::BOATHOUSE:
		buffer.push_back(new BoatHouse(team));
		break;
	}	

	return *(buffer.end() - 1);
}

void j1EntityManager::DeleteEntity(Entity* entity_)
{
	if(entities.size() != 0)
		for (auto entity = entities.begin(); entity != entities.end(); entity++)
		{
			if ((*entity) == entity_)
			{
				delete (*entity);
				entities.erase(entity, entity + 1);
				entities.shrink_to_fit();
				break;
			}
		}
}

void j1EntityManager::QuickDeleteEntity(std::vector<Entity*>::iterator entity)
{
	delete (*entity);
	entities.erase(entity, entity + 1);
	entities.shrink_to_fit();
}

void j1EntityManager::DeleteAll()
{
	while (entities.size() != 0)
	{
		delete (*entities.begin());
		entities.erase(entities.begin(), entities.begin() + 1);
		entities.shrink_to_fit();
	}
}

bool j1EntityManager::CleanUp()
{
	return true;
}