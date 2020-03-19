#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1SceneManager.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Transitions.h"
#include "j1TransitionManager.h"


j1SceneManager::j1SceneManager() : j1Module()
{
	name.create("scenemanager");
}

// Destructor
j1SceneManager::~j1SceneManager()
{}

// Called before render is available
bool j1SceneManager::Awake()
{

	return true;
}

// Called before the first frame
bool j1SceneManager::Start()
{
	
	return true;
}

// Called each loop iteration
bool j1SceneManager::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool j1SceneManager::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		current_scene = ChangeScene(1);

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		current_scene = ChangeScene(2);
	
	return true;
}


// Called before quitting
bool j1SceneManager::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

int j1SceneManager::ChangeScene(int scene)
{
	switch (scene)
	{
	case 1:
		App->scene->active = true;
		App->scene2->active = false;
		return 1;
		break;
	case 2:
		App->scene->active = false;
		App->scene2->active = true;
		return 2;
		break;
	
	}
	return 0;
}
