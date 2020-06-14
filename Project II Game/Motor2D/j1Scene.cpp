#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1PathFinding.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Map.h"
#include "j1GUI.h"
#include "j1Transitions.h"
#include "j1TransitionManager.h"
#include "j1EntityManager.h"
#include "j1LogoScene.h"
#include "j1InGameUI.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	state = ONGOING;

	if (App->restart == true)
	{
		App->render->camera.x = 5192;
		App->render->camera.y = -2904;
	}
	App->restart = true;

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	switch (state)
	{
	case WIN:
		App->InGameUI->Activate_Win_Menu();
		break;
	case LOSE:
		App->InGameUI->Activate_Defeat_Menu();
		break;
	}

	App->map->Draw();

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	switch (state)
	{
	case WIN:
		App->InGameUI->Deactivate_Win_Menu();
		break;
	case LOSE:
		App->InGameUI->Deactivate_Defeat_Menu();
		break;
	}
		
	return true;
}

void j1Scene::ChangeScene()
{
	this->active = true;
	App->map->LoadMap("maps/Mapa definitivo.tmx");
	App->scene2->active = false;
	App->scene3->active = false;
	App->scene->Start();
}
