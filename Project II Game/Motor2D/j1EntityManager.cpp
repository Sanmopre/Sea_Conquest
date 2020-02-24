#include "j1EntityManager.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "j1App.h"

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

	return (*entities.end());
}

void j1EntityManager::DeleteEntity(Entity* entity)
{

}