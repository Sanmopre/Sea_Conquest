#ifndef __j1QUESTMANAGER_H__
#define __j1QUESTMANAGER_H__

#include "j1Module.h"

struct SDL_Texture;

enum QUEST{
KILL_15_BOATS,
GATHER_1000_IRON,
KILL_ENEMY_BASE,
BUILD_10_BOATS,
NONE
};

struct QuestManagerUI {
	j1Element* image_open;
	j1Element* image_close;
	j1Element* button;
};

class j1QuestManager : public j1Module
{
public:

	j1QuestManager();
	virtual ~j1QuestManager();

	bool Awake(pugi::xml_node& config);

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void Set_Quest(QUEST quest = QUEST::NONE);
	void Restart_Quest(QUEST quest = QUEST::NONE);
	void Cancel_Quest(QUEST quest = QUEST::NONE);
	void Finish_Quest(QUEST quest = QUEST::NONE);
public:

private:
	QuestManagerUI manager;
};


#endif // __j1QUESTMANAGER_H__
