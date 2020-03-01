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
#include "j1GUI.h"

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
	App->map->Load("iso_walk.tmx") == true;
	/*/
	if(App->map->Load("este fk men.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if(App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}//*/
	Add_UI();
	
	debug_tex = App->tex->Load("maps/path2.png");

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if(origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);

	//App->win->SetTitle(title.GetString());

	// Debug pathfinding ------------------------------
	//int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	//App->render->Blit(debug_tex, p.x, p.y);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for(uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		//App->render->AddBlitEvent(0,debug_tex, pos.x, pos.y);
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}


//UI FUNCTIONS
void j1Scene::Add_UI()
{
	menu.Menu_button = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, { 1400,20 }, { 0,0 }, true, true, { 0,0,100,50 }, "OPTIONS", this);
	menu.Return_button = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, { 610,430 }, { 00,0 }, true, false, { 0, 0,130,22 }, "RETURN", this);
	menu.Title = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, { 605,300 }, { 0,0 }, false, false, { 0, 0,139,27 }, "MENU", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	menu.Resume_button = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, { 610,400 }, { 0,0 }, true, false, { 0, 0,130,22 }, "RESUME", this);
	menu.Exit_button = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, { 610,460 }, { 0,0 }, true, false, { 0, 0,130,22 }, "EXIT", this);
	menu.Save = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, { 610,520 }, { 0,0 }, true, false, { 0, 0,130,22 }, "SAVE", this);
	menu.Load = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, { 610,490 }, { 0,0 }, true, false, { 0, 0,130,22 }, "LOAD", this);
}


void j1Scene::Activate_Menu()
{
	menu.Resume_button->enabled = !menu.Resume_button->enabled;
	menu.Return_button->enabled = !menu.Return_button->enabled;
	menu.Title->enabled = !menu.Title->enabled;
	menu.Exit_button->enabled = !menu.Exit_button->enabled;
	menu.Load->enabled = !menu.Load->enabled;
	menu.Save->enabled = !menu.Save->enabled;
}

void j1Scene::GUI_Event_Manager(GUI_Event type, j1GUIelement* element)
{
	switch (type)
	{

	case GUI_Event::EVENT_ONCLICK:
	{
		
		if (element == menu.Return_button) {
			Activate_Menu();
		}

		if (element == menu.Exit_button) {
			Activate_Menu();
		}

		if (element == menu.Save) {
			Activate_Menu();
		}
		if (element == menu.Load) {
			Activate_Menu();
		}

		if (element == menu.Resume_button) {
			Activate_Menu();
		}
		if (element == menu.Menu_button) {
			Activate_Menu();
		}

		if (element == menu.Exit_button) {
			Activate_Menu();
		}
	}
	}
}