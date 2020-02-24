#include "j1EntityManager.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "j1App.h"

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
	LOG("EM Updating");
	return true;
}

bool j1EntityManager::CleanUp()
{
	return true;
}

Entity* j1EntityManager::AddEntity(float x, float y, Entity_Type type)
{
	Entity* entity = nullptr;

	return entity;
}

void j1EntityManager::DeleteEntity(Entity* entity)
{

}