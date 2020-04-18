#include "j1Minimap.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1SceneManager.h"

#include "p2Log.h"
#include <list>

j1Minimap::j1Minimap()
{
	name = "minimap";
}

j1Minimap::~j1Minimap() {}

bool j1Minimap::Awake(pugi::xml_node & config)
{
	position.x= config.attribute("position.x").as_int(1020);
	position.y= config.attribute("position.y").as_int(560);

	return true;
}

bool j1Minimap::Start()
{
	minimap_tex = App->tex->Load("textures/minimap.png");
	minimap_camera = App->tex->Load("textures/minimap_camera.png");
	return true;
}


bool j1Minimap::Update(float dt)
{	
	if (App->scenemanager->In_Logo_Scene != true && App->scenemanager->In_Main_Menu != true) {		
			MinimapToWorldCamera();
			DrawCamera();
		App->render->AddBlitEvent(5, minimap_tex, position.x - App->render->camera.x, position.y - App->render->camera.y, rect, false, true, 0u, 0u, 0u, 255, true);

	}
	
	return true;
}

bool j1Minimap::PostUpdate()
{	

	return true;
}

bool j1Minimap::CleanUp()
{
	SDL_DestroyTexture(minimap_tex);

	return true;
}




void j1Minimap::DrawCamera()
{
	App->render->AddBlitEvent(6, minimap_camera, 128 + position.x - App->render->camera.x/App->win->scale - (App->render->camera.x / 50) / App->win->scale, position.y - App->render->camera.y / App->win->scale - (App->render->camera.y / 50) / App->win->scale, cameraminimap, false);
}

void j1Minimap::MinimapToWorldCamera()
{
	int x, y = 0;
	App->input->GetMousePosition(x,y);
	if (x > position.x&& y > position.y) {
		if (App->input->GetMouseButtonDown(1) == KEY_DOWN) {
			clicking_map = true;
			App->render->camera.x = - ((x - position.x) * 50 - 6400) + 640;
			App->render->camera.y = - ((y - position.y) * 50 ) + 360;
		}
	}
	else {
		clicking_map = false;
	}
}



