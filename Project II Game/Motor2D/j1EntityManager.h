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
	bool Update();
	bool CleanUp();

	Entity* AddEntity(float x, float y, Entity_Type, int level);
	void DeleteEntity(Entity*);
	void DeleteAll();

	std::vector<Entity*> entities;
	Entity* enttest;
};

#endif // __j1EntityManager_H__
