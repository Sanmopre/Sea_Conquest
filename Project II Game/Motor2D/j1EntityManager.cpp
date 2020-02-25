#include "j1EntityManager.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "j1App.h"

#include "j1Input.h"
#include "j1Render.h"

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

bool j1EntityManager::Update()
{
	for (vector<Entity*>::iterator entity = entities.begin(); entity != entities.end(); entity++)
	{
		(*entity)->Update(6.9f);
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		iPoint test;
		App->input->GetMousePosition(test.x, test.y);
		test.x -= App->render->camera.x;
		test.y -= App->render->camera.y;
		enttest = AddEntity(test.x, test.y, Entity_Type::BOAT, 1);
		LOG("There is %d entities with %d vector capacity", entities.size(), entities.capacity());
	}

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		DeleteEntity(enttest);
		enttest->position.x += 10;
		LOG("There is %d entities with %d vector capacity", entities.size(), entities.capacity());
	}
	return true;
}

bool j1EntityManager::CleanUp()
{
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

void j1EntityManager::DeleteEntity(Entity* _entity)
{
	for (auto entity = entities.begin(); entity != entities.end(); entity++)
	{
		if (*entity == _entity)
		{
			LOG("Deleting entity Kapp");
			//delete *entity;
			//entities.erase(entity);
			//entities.shrink_to_fit();
		}
	}
}