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
#include "j1LogoScene.h"
#include "j1GUI.h"
#include "j1Transitions.h"
#include "j1TransitionManager.h"

j1Scene2::j1Scene2() : j1Module()
{
	name.create("scene2");
}

// Destructor
j1Scene2::~j1Scene2()
{}

// Called before render is available
bool j1Scene2::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene2::Start()
{

	return true;
}

// Called each loop iteration
bool j1Scene2::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool j1Scene2::Update(float dt)
{
	App->render->AddBlitEvent(1, main_texture, 0, 0, { 0,0,1280,720 }, false, true, 0u, 0u, 0u, 255, true);

	return true;
}

bool j1Scene2::PostUpdate()
{


	return true;
}

bool j1Scene2::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene2::ChangeScene() {
	
	this->active = true;
	App->scene->active = false;
	App->scene3->active = false;
	App->scene2->Start();
}