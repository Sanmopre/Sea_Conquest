#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1SceneManager.h"
#include "j1LogoScene.h"
#include "j1GUI.h"
#include "j1Transitions.h"
#include "j1TransitionManager.h"

j1LogoScene::j1LogoScene() : j1Module()
{
	name.create("Logoscene");
}

// Destructor
j1LogoScene::~j1LogoScene()
{}

// Called before render is available
bool j1LogoScene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1LogoScene::Start()
{

	return true;
}

// Called each loop iteration
bool j1LogoScene::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool j1LogoScene::Update(float dt)
{
	if(App->scenemanager->In_Logo_Scene == true)
	App->render->AddBlitEvent(1, logo_texture, 0, 0, { 0,0,1280,720 }, false, true, 0u, 0u, 0u, 255, true);

	return true;
}

bool j1LogoScene::PostUpdate()
{


	return true;
}

bool j1LogoScene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1LogoScene::ChangeScene() {

	this->active = true;
	if (App->scene->active)
	{
		App->scene->CleanUp();
		App->scene->active = false;
	}
	App->scene2->active = false;
	App->scene3->Start();
}