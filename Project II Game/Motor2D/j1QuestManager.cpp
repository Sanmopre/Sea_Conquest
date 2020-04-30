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
