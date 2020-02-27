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
	for (vector<Entity*>::iterator entity = entities.begin(); entity != entities.end(); entity++)
	{
		if ((*entity)->to_delete)
		{
			//NOT THE YATTAS !!!
		}
		else
			(*entity)->Update(6.9f);
	}
	////////////////////////////////////ENTITIES_DEBUG///////////////////////////////////////////////////
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		iPoint test;
		App->input->GetMousePosition(test.x, test.y);
		test.x -= App->render->camera.x / App->win->GetScale();
		test.y -= App->render->camera.y / App->win->GetScale();
		enttest = AddEntity(test.x, test.y, Entity_Type::BOAT, 1);

		LOG("There is %d entities with %d vector capacity", entities.size(), entities.capacity());
	}

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		DeleteEntity(enttest);
		if(entities.size() != 0)
			enttest = *(entities.end() - 1);
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

Entity* j1EntityManager::AddEntity(float x, float y, Entity_Type type, int level)
{
	switch (type)
	{
	case Entity_Type::BOAT:
		entities.push_back(new Boat(x, y, level));
	}	

	return *(entities.end() - 1);
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