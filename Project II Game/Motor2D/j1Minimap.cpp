#include "j1Minimap.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Textures.h"
#include "j1Input.h"

#include "j1SceneManager.h"
#include "j1Entities.h"
#include "j1EntityManager.h"

#include "p2Log.h"
#include <list>

j1Minimap::j1Minimap()
{
	name = "minimap";
}

j1Minimap::~j1Minimap() {}

bool j1Minimap::Awake(pugi::xml_node & config)
{
	position.x= config.attribute("position.x").as_int(1013);
	position.y= config.attribute("position.y").as_int(560);

	return true;
}

bool j1Minimap::Start()
{
	minimap_tex = App->tex->Load("textures/minimap.png");
	minimap_camera = App->tex->Load("textures/minimap_camera.png");

	enemy = App->tex->Load("textures/enemy_unit.png");
	ally = App->tex->Load("textures/friend_unit.png");

	cotton = App->tex->Load("textures/cotton_mini.png");
	metal = App->tex->Load("textures/metal_mini.png");
	wood = App->tex->Load("textures/wood_mini.png");
	return true;
}


bool j1Minimap::Update(float dt)
{	
	if (App->scenemanager->In_Logo_Scene != true && App->scenemanager->In_Main_Menu != true) {
			MinimapToWorldCamera();
	//	App->render->AddBlitEvent(5, minimap_tex, position.x - App->render->camera.x, position.y - App->render->camera.y, rect, false, true, 0u, 0u, 0u, 255, true);
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
	App->render->AddBlitEvent(6, minimap_camera, 128 + position.x - App->render->camera.x - (App->render->camera.x / 50), position.y - App->render->camera.y - (App->render->camera.y / 50), cameraminimap, false);
}

void j1Minimap::MinimapToWorldCamera()
{
	int x, y = 0;
	App->input->GetMousePosition(x,y);
	if (x > position.x&& y > position.y) {
		if (App->input->GetMouseButtonDown(1) == KEY_REPEAT) {
			clicking_map = true;
			App->render->camera.x = - ((x - position.x) * 50 - 6400) + 640;
			App->render->camera.y = - ((y - position.y) * 50 ) + 360;
		}
	}
	else {
		clicking_map = false;
	}
}

void j1Minimap::Draw_entities()
{

	for (std::vector<j1Entity*>::iterator i = App->entitymanager->entities.begin(); i != App->entitymanager->entities.end(); i++)
	{
		if ((*i)->team == 1)
		{
			
			App->render->AddBlitEvent(6, ally,128 +  position.x + (*i)->position.x/50 - App->render->camera.x, position.y + (*i)->position.y / 50 - App->render->camera.y ,unit, false);

		}
		else if ((*i)->team == 2) {

			App->render->AddBlitEvent(6, enemy, 128 + position.x + (*i)->position.x / 50 - App->render->camera.x, position.y + (*i)->position.y / 50 - App->render->camera.y , unit, false);

		}
		else if ((*i)->type == EntityType::ALL_COTTON) {

			App->render->AddBlitEvent(6, cotton, 128 + position.x + (*i)->position.x / 50 - App->render->camera.x, position.y + (*i)->position.y / 50 - App->render->camera.y, unit, false);

		}
		else if ((*i)->type == EntityType::ALL_WOOD) {

			App->render->AddBlitEvent(6, wood, 128 + position.x + (*i)->position.x / 50 - App->render->camera.x, position.y + (*i)->position.y / 50 - App->render->camera.y, unit, false);

		}
		else if ((*i)->type == EntityType::ALL_METAL) {

			App->render->AddBlitEvent(6, metal, 128 + position.x + (*i)->position.x / 50 - App->render->camera.x, position.y + (*i)->position.y / 50 - App->render->camera.y, unit, false);

		}
			
	}
}



