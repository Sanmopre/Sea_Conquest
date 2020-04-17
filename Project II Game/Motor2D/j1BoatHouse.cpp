#include "j1Entities.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Input.h"

#include "j1EntityManager.h"
#include "j1Map.h"
#include "j1InGameUI.h"
#include "j1Scene.h"

j1BoatHouse::j1BoatHouse(float x, float y, int team)
{
	type = EntityType::BOATHOUSE;
	main_type = EntityType::STRUCTURE;

	tile = App->map->WorldToMap(x, y);
	position = App->map->MapToWorld<fPoint>(tile.x, tile.y);

	level = 1;
	this->team = team;
	max_health = 500;
	if(built_state != BUILDING)
		health = max_health;
	rect = { 128, 0, 64, 64 };
	built_rect = rect; /////////////////////
	load = { 0, 0, 0, 1000 };
	texture = LoadTexture(this, App->entitymanager->allTextures);
}

j1BoatHouse::~j1BoatHouse()
{

}

void j1BoatHouse::Update(float dt)
{
	if (selected)
	{
		if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
			BuildUnit(EntityType::BOAT, 1);
	}

	App->render->AddBlitEvent(1, texture, GetRenderPositionX(), GetRenderPositionY(), rect);

	if (health == 0)
		CleanUp();
}



void j1BoatHouse::CleanUp()
{
	to_delete = true;
}
