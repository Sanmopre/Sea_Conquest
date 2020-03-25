#ifndef __j1EntityManager_H__
#define __j1EntityManager_H__

#include "j1Module.h"
#include "j1Entities.h"
#include <vector>

enum class SearchType
{
	SELECTED,
	TEAM,
	AREA
};

struct SearchRequest
{
	SearchRequest(std::vector<j1Entity*>* result, bool list, int selected, int team, SearchType type, fPoint searcher_position, int range, bool circular)
	{
		this->result = result;
		this->list = list;
		this->selected = selected;
		this->team = team;
		this->type = type;
		this->searcher_position = searcher_position;
		this->range = range;
		this->circular = circular;
	}
	std::vector<j1Entity*>* result;
	bool list;
	int selected;
	int team;
	SearchType type;
	fPoint searcher_position;
	int range;
	bool circular;
};

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	j1Entity* AddEntity(float x = 0, float y = 0, EntityType = EntityType::NONE, int level = 1, int team = 0);

	void SearchEntity(std::vector<j1Entity*>* result, int selected = 2, int team = 0);
	void SearchEntity(std::vector<j1Entity*>* result, int team);
	void SearchEntity(std::vector<j1Entity*>* result, fPoint searcher_position, int range, bool circular, int team, int selected = 2);

	void DeleteEntity(j1Entity*);
	void DeleteAll();
	void addAnimation(Animation);
	void addTexture(TextureInfo);

	std::vector<j1Entity*> entities;
	std::vector<Animation> allAnimations;
	std::vector<TextureInfo> allTextures;
	std::vector<j1Entity*> selected_list;

private:

	std::vector<j1Entity*> buffer;
	void QuickDeleteEntity(std::vector<j1Entity*>::iterator itr);

	std::vector<SearchRequest> requests;
	void AddSearchRequest(std::vector<j1Entity*>* result, bool list, int selected, int team, SearchType type, fPoint searcher_position, int range, bool circular);
};

#endif // __j1EntityManager_H__
