#include "j1Entities.h"
#include "j1Render.h"

#include "j1EntityManager.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1InGameUI.h"
#include "j1Input.h"

j1Storage::j1Storage(float x, float y, int team)
{
	if (team != 1 || App->scene->start)
	{
		placed = true;
		tile = App->map->WorldToMap(x, y);
		position = App->map->MapToWorld<fPoint>(tile.x, tile.y);
	}
	else
		position = { x,y };

	//rect = { (int)position.x, (int)position.y, 30, 30 };
	rect = { 0, 0, 64, 64 };
	this->team = team;
	load = { 0,0,0,2000 };
	int range = 100;
	trading_range = 200;
	max_health = 250;
	health = max_health;
	
	type = EntityType::STORAGE;
	level = 1;

	texture = LoadTexture(this, App->entitymanager->allTextures);
}

j1Storage::~j1Storage()
{

}

void j1Storage::Update(float dt)
{
	showing_hpbar = false;

	NotPlacedBehaviour();

	if(team == 1)
		if (selected)
		{
			ShowHPbar(10, 5);

			if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
				BuildUnit(EntityType::HARVESTER, 1);

			if (this == App->InGameUI->selected)
				Trading();
		}

	BuildProcces(dt);

	//App->render->AddBlitEvent(1, nullptr, 0, 0, rect, false, false, 100, 0, 255);
	App->render->AddBlitEvent(1, texture, GetRenderPositionX(), GetRenderPositionY(), rect);
}

void j1Storage::CleanUp()
{
	to_delete = true;
}