#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1SceneManager.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Transitions.h"
#include "j1EntityManager.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Textures.h"
#include "j1InGameUI.h"
#include "j1Window.h"
#include "j1MainMenuUI.h"
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
	//LOAD THE START OF BOTH SCENES
	App->map->Load("mapa men.tmx") == true;
	App->scene2->main_texture = App->tex->Load("textures/Main_Screen.png");
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
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN){
		App->transitions->LinesAppearing(Black, 0.75f, 1);
		App->mainmenu->Deactivate_Audio_Options();
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
		App->entitymanager->DeleteAll();
		App->transitions->LinesAppearing(Black, 0.75f, 2);
		App->mainmenu->Deactivate_Audio_Options();
	}

	/*
	App->transitions->FadingToColor(White, 1.0f, //scene);
	App->transitions->Wiping(Black, 0.5f, //scene);
	App->transitions->LinesAppearing(Black, 1.0f, //scene);
	App->transitions->SquaresAppearing(1, Black, 1.0f, //scene);
	App->transitions->SquaresAppearing(2, Black, 1.0f, //scene);
	App->transitions->SquaresAppearing(3, Black, 1.0f, //scene);
	App->transitions->SquaresAppearing(4, Black, 1.0f, //scene);
	*/

	if (In_Main_Menu == true) {
		App->InGameUI->Deactivate_All_UI();
		App->mainmenu->Activate_Menu();
	}
	else {
		App->InGameUI->Activate_Necessary_UI();
		App->mainmenu->Deactivate_Menu();
	}

	
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
		App->scene2->ChangeScene();
		In_Main_Menu = false;
		return 1;
		break;
	case 2:

		App->scene->ChangeScene();
		//Managing camera
		App->render->camera.x = 0;
		App->render->camera.y = 0;
		App->win->scale = 1;

		In_Main_Menu = true;
		return 2;
		break;
	}
	return 0;
}
