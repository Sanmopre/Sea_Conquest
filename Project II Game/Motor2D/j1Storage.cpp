#include "j1Entities.h"
#include "j1Render.h"

#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1InGameUI.h"
#include "j1Input.h"
#include "j1Scene.h"

j1Storage::j1Storage(float x, float y, int team)
{

	tile = App->map->WorldToMap(x, y);
	position = App->map->MapToWorld<fPoint>(tile.x, tile.y);
	terrain = NodeType::GROUND;
	level = 1;
	this->team = team;

	texture = App->tex->GetTexture("storage", level, 0);
	tex_construction = App->tex->GetTexture("cons_medium", 0, 0);

	basic = App->anim->GetAnimation("ally_storage_empty");
	low = App->anim->GetAnimation("ally_storage_low");
	half = App->anim->GetAnimation("ally_storage_half");
	full = App->anim->GetAnimation("ally_storage_full");
	under_construction = App->anim->GetAnimation("cons_medium");

	
	load = { 0,0,0,2000 };
	int range = 100;
	trading_range = 200;
	max_health = 250;
	if (built_state != BUILDING)
	{
		health = max_health;
		(*App->pathfinding->WorldToNode(tile.x, tile.y))->built = true;
	}
	type = EntityType::STORAGE;
}

j1Storage::~j1Storage()
{

}

void j1Storage::Update(float dt)
{
	bool flip = false;
	if (team == 1)
	{
		flip = true;
		if (selected)
		{

			if (App->godmode)
			{
				if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
					BuildUnit(EntityType::HARVESTER, 1);
			}
		}
	}

	current_animation = &basic;

	float percent = 100.0f / load.maxweight;
	percent *= load.Weight();

	if (percent > 0 && percent < 50)
		current_animation = &low;
	else if (percent >= 50 && percent < 100)
		current_animation = &half;
	else if (percent >= 100)
		current_animation = &full;
	

	App->render->AddBlitEvent(1, texture, GetRenderPositionX(), GetRenderPositionY(), rect, flip);
}

void j1Storage::CleanUp()
{
	to_delete = true;
}