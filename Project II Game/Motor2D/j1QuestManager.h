#ifndef __j1QUESTMANAGER_H__
#define __j1QUESTMANAGER_H__

#include "j1Module.h"
#include "p2Point.h"

struct SDL_Texture;

enum QUEST{
KILL_15_BOATS,
GATHER_1000_IRON,
KILL_ENEMY_BASE,
BUILD_10_BOATS,
NONE
};

struct Quest {
	int total;
	char total_t[5];
	int current;
	char current_t[5];
	int reward;
	char reward_t[5];

	j1Element* text;
	j1Element* image;
	j1Element* reward_image;
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

	QUEST Set_Quest(QUEST quest = QUEST::NONE);
	void Restart_Quest(QUEST quest = QUEST::NONE);
	void Cancel_Quest(QUEST quest = QUEST::NONE);
	void Finish_Quest(QUEST quest = QUEST::NONE);


	void Close_Quest_Manager();
	void Open_Quest_Manager();
	void GUI_Event_Manager(GUI_Event type, j1Element* element);
public:
	bool quest_manager_open = false;
	QUEST current_quest = QUEST::NONE;
private:
	QuestManagerUI manager;
	Quest main_quest;
};


#endif // __j1QUESTMANAGER_H__
