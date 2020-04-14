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
#include "j1EntityManager.h"

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

	//App->map->Load("mapa men.tmx") == true;
	App->render->camera.x = -(-5050) * App->win->GetScale();
	App->render->camera.y = -(2850) * App->win->GetScale();
	App->entitymanager->AddEntity(-4500.0f, 3100.0f, EntityType::TOWNHALL);
	App->entitymanager->AddEntity(-4470.0f, 3150.0f, EntityType::STORAGE);
	App->entitymanager->AddEntity(-4590.0f, 3470.0f, EntityType::RESOURCE);
	debug_tex = App->tex->Load("maps/path2.png");
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

	return true;
}

void j1Scene::ChangeScene()
{
	this->active = false;
	CleanUp();

	App->scene2->active = true;
	App->scene2->Start();
}
