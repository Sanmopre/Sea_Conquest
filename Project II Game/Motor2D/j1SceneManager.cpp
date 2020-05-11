#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1SceneManager.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1LogoScene.h"
#include "j1Transitions.h"
#include "j1EntityManager.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1InGameUI.h"
#include "j1Window.h"
#include "j1MainMenuUI.h"
#include "j1TransitionManager.h"
#include "j1Audio.h"
#include "j1Player.h"
#include "j1Pathfinding.h"
#include "j1Map.h"

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
	App->scene2->main_texture = App->tex->Load("textures/Main_Screen.png");
	App->scene3->logo_texture = App->tex->Load("textures/logo.png");
	logo.iterations = 3.5f;
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
	//DEBUG ONLY
	/*
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)	{
		App->transitions->LinesAppearing(Black, 1.25f, 1);
		App->mainmenu->Deactivate_Audio_Options();
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
		App->transitions->LinesAppearing(Black, 1.25f, 2);
		App->mainmenu->Deactivate_Audio_Options();
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) {
		App->transitions->FadingToColor(White, 1.25f, 3);
		App->mainmenu->Deactivate_Audio_Options();
	}*/





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
		App->InGameUI->CleanUp();
		App->player->CleanUp();
		App->mainmenu->Activate_Menu();
		App->godmode = false;
	}
	else {
		App->InGameUI->Activate_Necessary_UI();
		App->mainmenu->Deactivate_Menu();
	}

	if (In_Logo_Scene == true) {
		App->mainmenu->Deactivate_Menu();
		App->InGameUI->Deactivate_All_UI();
		App->InGameUI->CleanUp();
		App->player->CleanUp();
		App->godmode = false;
	}
	//LOGO_SCENE_TIMER
	
	if(finished_logo != true){
	logo.counter += dt;
	if (logo.counter >= logo.iterations) {
		finished_logo = true;
		App->transitions->FadingToColor(White, 1.0f, 2);
	}
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
		App->pathfinding->show = false;
		App->scene->active = true;
		App->scene->ChangeScene();
		//App->audio->PlayMusic("audio/music/Sea_conquest_chill_cutre.wav", 2.0f, App->audio->ingame_chill_music);
		App->audio->StopFx(-1);
		//App->audio->PlayFx(App->audio->ingame_chill_music, -1);
		App->audio->PlayMusic(App->audio->ingame_chill_music, 0.0f);
		In_Main_Menu = false;
		In_Logo_Scene = false;
		return 1;
		break;
	case 2:		
		App->pathfinding->show = false;
		App->scene2->active = true;
		App->scene2->ChangeScene();
		//App->audio->PlayMusic("audio/music/Motorista_Reciclista_Shop1_start.wav", 2.0f, App->audio->mainmenu_music);
		App->audio->StopFx(-1);
		//App->audio->PlayFx(App->audio->mainmenu_music, -1);
		App->audio->PlayMusic(App->audio->mainmenu_music, 0.0f);
		App->audio->PlayFx(App->audio->welcome_voice, 0.0f);
		App->render->camera.x = 0;
		App->render->camera.y = 0;
		App->win->scale = 1;
		In_Logo_Scene = false;
		In_Main_Menu = true;
		App->map->CleanUp();
		return 2;
		break;
	case 3:
		App->scene3->active = true;
		App->scene3->ChangeScene();
		App->render->camera.x = 0;
		App->render->camera.y = 0;
		App->win->scale = 1;
		In_Logo_Scene = true;
		In_Main_Menu = true;
		return 3;
		break;
	}
	return 0;
}
