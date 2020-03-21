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
	App->map_2->Load("iso.tmx") == true;

	debug_tex = App->tex->Load("maps/path2.png");

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
	App->map->Draw();

	return true;
}

bool j1Scene2::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool j1Scene2::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene2::ChangeScene() {

	this->active = false;
	App->map_2->CleanUp();
	CleanUp();


	App->scene->active = true;
	App->scene->Start();
}