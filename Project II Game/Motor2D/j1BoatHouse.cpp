#include "j1Entities.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Input.h"

#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1InGameUI.h"
#include "j1Scene.h"
#include "j1Minimap.h"

j1BoatHouse::j1BoatHouse(float x, float y, int team)
{
	type = EntityType::BOATHOUSE;
	main_type = EntityType::STRUCTURE;
	terrain = NodeType::WATER;
	level = 1;
	this->team = team;
	fog_range = 4;

	texture = App->tex->GetTexture("boathouse", level, 0);
	tex_construction = App->tex->GetTexture("cons_small", 0, 0);

	basic = App->anim->GetAnimation("boathouse");

	under_construction = App->anim->GetAnimation("cons_small");

	tile = App->map->WorldToMap(x, y);
	position = App->map->MapToWorld<fPoint>(tile.x, tile.y);

	max_health = 500;
	if (built_state != BUILDING)
		health = max_health;

	(*App->pathfinding->WorldToNode(tile.x, tile.y))->built = true;
	(*App->pathfinding->WorldToNode(tile.x + 1, tile.y))->built = true;
	(*App->pathfinding->WorldToNode(tile.x, tile.y + 1))->built = true;
	(*App->pathfinding->WorldToNode(tile.x + 1, tile.y + 1))->built = true;

	current_animation = &basic;

	load = { 0, 0, 0, 0 };
}

j1BoatHouse::~j1BoatHouse()
{
	if (App->pathfinding->NodeMap.size() != 0) 
	{
		(*App->pathfinding->WorldToNode(tile.x + 1, tile.y))->built = false;
		(*App->pathfinding->WorldToNode(tile.x, tile.y + 1))->built = false;
		(*App->pathfinding->WorldToNode(tile.x + 1, tile.y + 1))->built = false;
	}
}

void j1BoatHouse::Update(float dt)
{
	if (selected)
	{
		if (App->godmode)
		{
			if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
				BuildUnit(EntityType::BOAT, 1);
			if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
				BuildUnit(EntityType::CARRIER, 1);
			if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
				BuildUnit(EntityType::BALLOON, 1);
		}
	}

	if (built_state != BUILDING && built_state != ON_HOLD)
		current_animation = &basic;

	if (App->fog->GetVisibility(tile.x, tile.y) == FogState::VISIBLE || App->godmode)
	{
		App->render->AddBlitEvent(1, texture, GetRenderPositionX(), GetRenderPositionY(), rect);
		App->minimap->Draw_entities(this);
	}

	if (health == 0)
		CleanUp();
}



void j1BoatHouse::CleanUp()
{
	to_delete = true;
}
