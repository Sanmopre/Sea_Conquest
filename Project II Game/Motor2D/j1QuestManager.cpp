#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Font.h"
#include "j1QuestManager.h"
#include "j1Render.h"
#include "j1EntityManager.h"
#include "j1Entities.h"
#include "j1GUI.h"
#include "j1Audio.h"
#include "j1InGameUI.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1InGameUI.h"
#include "SDL\include\SDL.h"


j1QuestManager::j1QuestManager() : j1Module()
{
	name.create("fonts");
}

// Destructor
j1QuestManager::~j1QuestManager()
{}

// Called before render is available
bool j1QuestManager::Awake(pugi::xml_node& conf)
{
	bool ret = true;

	return ret;
}

bool j1QuestManager::Start()
{
	//Quest manager position
	manager.button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 950 + 180,15 }, { 0,0 }, true, false, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::QUEST_CLOSE);
	manager.image_close = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 950 ,15 }, { 0,0 }, true, false, { 0, 0,180,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::QUEST_IMAGE_CLOSE);
	manager.image_open = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 950 ,15 + 30 }, { 0,0 }, true, false, { 0, 0,180,180 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::QUEST_IMAGE_OPEN);
	manager.current = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 965, 110 }, { 0,0 }, true, true, { 0,0,40,40 }, "CURRENT :", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	manager.total = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 965, 150 }, { 0,0 }, true, true, { 0,0,40,40 }, "OBJECTIVE :", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	manager.reward = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 965, 190 }, { 0,0 }, true, true, { 0,0,40,40 }, "REWARD :", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	manager.no_quest = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 985, 80 }, { 0,0 }, true, true, { 0,0,40,40 }, "NO QUEST ACTIVE!", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	quest_explanation_build_boat = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 995, 70 }, { 0,0 }, true, true, { 0,0,40,40 }, "BUILD 10 BOATS", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	quest_explanation_kill_boat = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 985, 70 }, { 0,0 }, true, true, { 0,0,40,40 }, "DESTROY 15 ENEMY BOATS", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	quest_explanation_destroy_structure = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 970, 70 }, { 0,0 }, true, true, { 0,0,40,40 }, "DESTROY ENEMY STRUCTURE", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);

	build_harvester = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 970, 70 }, { -10,0 }, true, false, { 0,0,40,40 }, "CREATE HARVESTER AT STORAGE", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	gather_60_metal = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 970, 70 }, { -10,0 }, true, false, { 0,0,40,40 }, "GATHER 60 METAL", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	build_boat = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 970, 70 }, { -10,0 }, true, false, { 0,0,40,40 }, "CREATE BOAT AT BOATHOUSE", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);

	gold_icon = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 1090,185 }, { 0,0 }, true, false, { 0, 0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::COIN);


	current_quest = Set_Quest(QUEST::CREATE_HARVESTER);

	return true;
}

bool j1QuestManager::Update(float dt)
{

	//MANAGE QUEST NUMBERS
	sprintf_s(main_quest.total_t, 10, "%7d", main_quest.total);
	sprintf_s(main_quest.reward_t, 10, "%7d", main_quest.reward);
	sprintf_s(main_quest.current_t, 10, "%7d", main_quest.current);

	//MANAGE NUMBERS WHEN QUEST I AVALIBLE AND OPEN
	if (quest_manager_open == true && quest_activate == true) {
		manager.current->enabled = true;
		manager.reward->enabled = true;
		manager.total->enabled = true;
		gold_icon->enabled = true;
		Quest_Line(current_quest);
		App->fonts->BlitText(1005, 110, 1, main_quest.current_t);
		App->fonts->BlitText(1005, 150, 1, main_quest.total_t);
		App->fonts->BlitText(970, 190, 1, main_quest.reward_t);


		//DEACTIVATE NO QUEST MESSAGE
		manager.no_quest->enabled = false;
	}

	//MANAGES WHEN EVERYTHING IS CLOSED AND NO QUEST
	if (quest_manager_open == false && quest_activate == false) {
		manager.current->enabled = false;
		manager.reward->enabled = false;
		manager.total->enabled = false;
		gold_icon->enabled = false;
		manager.no_quest->enabled = false;
		quest_explanation_build_boat->enabled = false;
		quest_explanation_kill_boat->enabled = false;
		quest_explanation_destroy_structure->enabled = false;

		build_harvester->enabled = false;
		gather_60_metal->enabled = false;
	}

	//MANAGES WHEN OPEN AND NO QUEST
	if (quest_manager_open == true && quest_activate == false) {
		manager.current->enabled = false;
		manager.reward->enabled = false;
		manager.total->enabled = false;
		manager.no_quest->enabled = true;
		gold_icon->enabled = false;
		quest_explanation_build_boat->enabled = false;
		quest_explanation_kill_boat->enabled = false;
		quest_explanation_destroy_structure->enabled = false;

		build_harvester->enabled = false;
		gather_60_metal->enabled = false;
	}



	//MANAGES WHEN IS CLOSED ALWAYS
	if (quest_manager_open == false) {
		manager.current->enabled = false;
		manager.reward->enabled = false;
		manager.total->enabled = false;
		gold_icon->enabled = false;
		manager.no_quest->enabled = false;
		quest_explanation_build_boat->enabled = false;
		quest_explanation_kill_boat->enabled = false;
		quest_explanation_destroy_structure->enabled = false;
		build_harvester->enabled = false;

		gather_60_metal->enabled = false;
	}


	if (Check_Quest_Stat(main_quest)) 
	current_quest = Set_Quest(QUEST::NONE);
	

	return true;
}

bool j1QuestManager::CleanUp()
{
	return false;
}

QUEST j1QuestManager::Set_Quest(QUEST quest)
{
	switch (quest)
	{
	case QUEST::BUILD_10_BOATS:
		main_quest.total = 10;
		main_quest.reward = 10;
		main_quest.current = 0;
		quest_activate = true;
		return QUEST::BUILD_10_BOATS;
		break;

	case QUEST::NONE:
		main_quest.total = 0;
		main_quest.reward = 0;
		main_quest.current = 0;
		quest_activate = false;
		return QUEST::NONE;
		break;

	case QUEST::KILL_15_BOATS:
		main_quest.total = 15;
		main_quest.reward = 20;
		main_quest.current = 0;
		quest_activate = true;
		return QUEST::KILL_15_BOATS;
		break;

	case QUEST::DESTROY_ENEMY_STRUCTURE:
		main_quest.total = 1;
		main_quest.reward = 15;
		main_quest.current = 0;
		quest_activate = true;
		return QUEST::DESTROY_ENEMY_STRUCTURE;
		break;


	//TUTORIAL
	case QUEST::CREATE_HARVESTER:
		main_quest.total = 1;
		main_quest.reward = 5;
		main_quest.current = 0;
		quest_activate = true;
		return QUEST::CREATE_HARVESTER;
		break;

	case QUEST::GATHER_60_METAL:
		main_quest.total = 60;
		main_quest.reward = 5;
		main_quest.current = 0;
		quest_activate = true;
		return QUEST::GATHER_60_METAL;
		break;

	case QUEST::BUILD_BOAT:
		main_quest.total = 1;
		main_quest.reward = 5;
		main_quest.current = 0;
		quest_activate = true;
		return QUEST::BUILD_BOAT;
		break;
	}

}


void j1QuestManager::Finish_Quest(QUEST quest)
{

}

void j1QuestManager::Select_Quest_Text(QUEST quest)
{

}

bool j1QuestManager::Check_Quest_Stat(Quest quest)
{
	if (quest.current >= quest.total) {
		Claim_Reward(quest);
		return true;
	}
	else
		return false;
}

void j1QuestManager::Claim_Reward(Quest quest)
{
	App->InGameUI->coins = App->InGameUI->coins + quest.reward;
}


void j1QuestManager::Quest_Line(QUEST quest)
{
	switch (quest)
	{
	case QUEST::BUILD_10_BOATS:
		quest_explanation_build_boat->enabled = true;
		quest_explanation_kill_boat->enabled = false;
		quest_explanation_destroy_structure->enabled = false;
		manager.no_quest->enabled = false;
		build_harvester->enabled = false;
		gather_60_metal->enabled = false;
		build_boat->enabled = false;
		break;

	case QUEST::KILL_15_BOATS:
		quest_explanation_build_boat->enabled = false;
		quest_explanation_kill_boat->enabled = true;
		quest_explanation_destroy_structure->enabled = false;
		manager.no_quest->enabled = false;
		build_harvester->enabled = false;
		gather_60_metal->enabled = false;
		build_boat->enabled = false;
		break;

	case QUEST::DESTROY_ENEMY_STRUCTURE:
		quest_explanation_destroy_structure->enabled = true;
		quest_explanation_build_boat->enabled = false;
		quest_explanation_kill_boat->enabled = false;
		manager.no_quest->enabled = false;
		build_harvester->enabled = false;
		gather_60_metal->enabled = false;
		build_boat->enabled = false;
		break;


	//TUTORIAL

	case QUEST::CREATE_HARVESTER:
		build_harvester->enabled = true;
		gather_60_metal->enabled = false;
		build_boat->enabled = false;
		quest_explanation_destroy_structure->enabled = false;
		quest_explanation_build_boat->enabled = false;
		quest_explanation_kill_boat->enabled = false;
		manager.no_quest->enabled = false;
		break;

	case QUEST::BUILD_BOAT:
		build_harvester->enabled = false;
		gather_60_metal->enabled = false;
		build_boat->enabled = true;
		quest_explanation_destroy_structure->enabled = false;
		quest_explanation_build_boat->enabled = false;
		quest_explanation_kill_boat->enabled = false;
		manager.no_quest->enabled = false;
		break;

	case QUEST::GATHER_60_METAL:
		build_harvester->enabled = false;
		gather_60_metal->enabled = true;
		build_boat->enabled = false;
		quest_explanation_destroy_structure->enabled = false;
		quest_explanation_build_boat->enabled = false;
		quest_explanation_kill_boat->enabled = false;
		manager.no_quest->enabled = false;
		break;

	case QUEST::NONE:
		build_harvester->enabled = false;
		gather_60_metal->enabled = false;
		build_boat->enabled = false;
		quest_explanation_destroy_structure->enabled = false;
		quest_explanation_build_boat->enabled = false;
		quest_explanation_kill_boat->enabled = false;
		manager.no_quest->enabled = true;
		break;
	}
}

void j1QuestManager::Close_Quest_Manager()
{
	manager.button->enabled = false;
	manager.image_close->enabled = false;
	manager.image_open->enabled = false;
	manager.current->enabled = false;
	manager.reward->enabled = false;
	manager.total->enabled = false;
	manager.no_quest->enabled = false;
	quest_explanation_build_boat->enabled = false;



	quest_manager_open = false;
}

void j1QuestManager::Open_Quest_Manager()
{
	manager.button->enabled = true;
	manager.image_close->enabled = true;
}

void j1QuestManager::GUI_Event_Manager(GUI_Event type, j1Element* element)
{
	switch (type)
	{

	case GUI_Event::EVENT_ONCLICK:
	{
		App->audio->PlayFx(App->audio->ui_wood_hit);
		if (element == manager.button) {
			manager.image_open->enabled = !manager.image_open->enabled;
			if (manager.image_open->enabled == true && quest_activate == true) {
				manager.no_quest->enabled = false;
				quest_manager_open = true;
			}
			else  if (manager.image_open->enabled == true && quest_activate == false) {
				manager.no_quest->enabled = true;
				quest_manager_open = true;
			}
			else {
				quest_manager_open = false;
			}
		}
	}
	break;
	}
}
