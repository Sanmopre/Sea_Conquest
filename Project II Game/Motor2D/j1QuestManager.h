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
	int total = 0;
	char total_t[10];
	int current = 0;
	char current_t[10];
	int reward = 0;
	char reward_t[10];

	j1Element* text;
	j1Element* image;
	j1Element* reward_image;
};

struct QuestManagerUI {
	j1Element* image_open;
	j1Element* image_close;
	j1Element* button;
	j1Element* current;
	j1Element* total;
	j1Element* reward;
	j1Element* no_quest;
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
	void Select_Quest_Text(QUEST quest = QUEST::NONE);

	void Quest_Line(QUEST quest = QUEST::NONE);
	void Close_Quest_Manager();
	void Open_Quest_Manager();
	void GUI_Event_Manager(GUI_Event type, j1Element* element);


public:
	bool quest_manager_open = false;
	bool quest_activate = false;
	QUEST current_quest = QUEST::NONE;
private:
	QuestManagerUI manager;
	Quest main_quest;

	j1Element* quest_explanation_build_boat = nullptr;
	j1Element* quest_explanation_kill_boat = nullptr;
};


#endif // __j1QUESTMANAGER_H__
