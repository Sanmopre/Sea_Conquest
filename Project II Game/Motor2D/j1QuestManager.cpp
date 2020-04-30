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
	manager.button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, {950 + 180,15}, { 0,0 }, true, false, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::QUEST_CLOSE);
	manager.image_close = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 950 ,15 }, { 0,0 }, true, false, { 0, 0,180,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::QUEST_IMAGE_CLOSE);
	manager.image_open = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 950 ,15 + 30 }, { 0,0 }, true, false, { 0, 0,180,180 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::QUEST_IMAGE_OPEN);

	return true;
}

bool j1QuestManager::Update(float dt)
{

	return true;
}

bool j1QuestManager::CleanUp()
{
	return false;
}

void j1QuestManager::Set_Quest(QUEST quest)
{
}

void j1QuestManager::Restart_Quest(QUEST quest)
{
}

void j1QuestManager::Cancel_Quest(QUEST quest)
{
}

void j1QuestManager::Finish_Quest(QUEST quest)
{
}

void j1QuestManager::Close_Quest_Manager()
{
	manager.button->enabled = false;
	manager.image_close->enabled = false;
	manager.image_open->enabled = false;
}

void j1QuestManager::Open_Quest_Manager()
{
	manager.button->enabled = true;
	manager.image_close->enabled =true;
}

void j1QuestManager::GUI_Event_Manager(GUI_Event type, j1Element* element)
{
	App->InGameUI->clicking_ui = true;
	switch (type)
	{

	case GUI_Event::EVENT_ONCLICK:
	{
		App->audio->PlayFx(App->audio->ui_wood_hit);
		if (element == manager.button) {
			manager.image_open->enabled = !manager.image_open->enabled;
		}
	}
	break;
	}
}
