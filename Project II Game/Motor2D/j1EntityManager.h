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

	bool Load(pugi::xml_node& data);
	bool Save(pugi::xml_node& data) const;

	j1Entity* AddEntity(float x = 0, float y = 0, EntityType = EntityType::NONE, int level = 1, int team = 0);
	void AddToBuffer(j1Entity*);

	void DeleteEntity(j1Entity*);
	void DeleteAll();

	vector<j1Entity*> entities;

	vector<j1Entity*> ally_entities;
	vector<j1Entity*> selected_units;
	vector<j1Entity*> air_units;

	bool InsideElipse(fPoint center, fPoint point, int range);

	int townhall_level;

private:

	vector<j1Entity*> buffer;
	vector<j1Entity*> for_deletion;
	void QuickDeleteEntity(std::vector<j1Entity*>::iterator itr);
};

#endif // __j1EntityManager_H__
