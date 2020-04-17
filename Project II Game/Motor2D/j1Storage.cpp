#include "j1Entities.h"
#include "j1Render.h"

#include "j1EntityManager.h"
#include "j1Map.h"
#include "j1InGameUI.h"
#include "j1Input.h"
#include "j1Scene.h"

j1Storage::j1Storage(float x, float y, int team)
{

	tile = App->map->WorldToMap(x, y);
	position = App->map->MapToWorld<fPoint>(tile.x, tile.y);

	rect = { 0, 0, 64, 64 };
	built_rect = rect; /////////////////////
	this->team = team;
	load = { 0,0,0,2000 };
	int range = 100;
	trading_range = 200;
	max_health = 250;
	if (built_state != BUILDING)
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
	if(team == 1)
		if (selected)
		{
			if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
				BuildUnit(EntityType::HARVESTER, 1);
		}

	App->render->AddBlitEvent(1, texture, GetRenderPositionX(), GetRenderPositionY(), rect);
}

void j1Storage::CleanUp()
{
	to_delete = true;
}