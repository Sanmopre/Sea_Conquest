#include "j1Entities.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Input.h"

#include "j1EntityManager.h"
#include "j1Map.h"
#include "j1InGameUI.h"

j1BoatHouse::j1BoatHouse(float x, float y, int team)
{
	type = EntityType::BOATHOUSE;
	main_type = EntityType::STRUCTURE;

	if (team != 1)
	{
		placed = true;
		tile = App->map->WorldToMap(x, y);
		position = App->map->MapToWorld<fPoint>(tile.x, tile.y);
	}
	else
		position = { x,y };
	
	level = 1;
	this->team = team;
	max_health = 500;
	health = max_health;
	//rect = { (int)position.x, (int)position.y, 40, 40 };
	rect = { 128, 0, 64, 64 };
	load = { 0, 0, 0, 1000 };
	texture = LoadTexture(this, App->entitymanager->allTextures);
}

j1BoatHouse::~j1BoatHouse()
{

}

void j1BoatHouse::Update(float dt)
{
	showing_hpbar = false;
	//rect.x = position.x;
	//rect.y = position.y;

	NotPlacedBehaviour();

	if (selected)
	{
		if (team == 0)
			color.SetColor(0u, 255u, 255u);
		else if (team == 1)
			color.SetColor(255u, 255u, 0u);

		if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
			placed = false;

		if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
			BuildUnit(EntityType::BOAT, 1);
		
		ShowHPbar(10, 5);

		if (this == App->InGameUI->selected)
			Trading();
	}
	else
		if (team == 1)
			color.Blue();
		else if (team == 2)
			color.Red();

	BuildProcces(dt);

	App->render->AddBlitEvent(1, texture, GetRenderPositionX(), GetRenderPositionY(), rect);

	if (health == 0)
		CleanUp();
}



void j1BoatHouse::CleanUp()
{
	to_delete = true;
}
