#ifndef __j1EntityManager_H__
#define __j1EntityManager_H__

#include "j1Module.h"
#include "j1Entities.h"
#include <vector>

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	Entity* AddEntity(int x = 0, int y = 0, Entity_Type = Entity_Type::NONE, int level = 1, int team = 0);
	void DeleteEntity(Entity*);
	void DeleteAll();

	std::vector<Entity*> entities;
	Entity* enttest;

private:

	void QuickDeleteEntity(std::vector<Entity*>::iterator itr);
};

#endif // __j1EntityManager_H__
