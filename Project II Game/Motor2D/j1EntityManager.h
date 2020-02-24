#ifndef __j1EntityManager_H__
#define __j1EntityManager_H__

#include "j1Module.h"
#include <vector>

enum class entity_type
{
	unit,
	structure,
	resources
};

class Entity;


class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	Entity* AddEntity(float x, float y, entity_type type);
	void DeleteEntity(Entity* entity);

	std::vector<Entity*> entities;
};

#endif // __j1EntityManager_H__
