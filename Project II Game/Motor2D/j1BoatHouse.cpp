#include "j1Entities.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Input.h"

#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1InGameUI.h"
#include "j1Scene.h"

j1BoatHouse::j1BoatHouse(float x, float y, int team)
{
	type = EntityType::BOATHOUSE;
	main_type = EntityType::STRUCTURE;
	terrain = NodeType::WATER;
	level = 1;
	this->team = team;

	texture = App->tex->GetTexture("boathouse", level, this->team);
	cons_tex = App->tex->GetTexture("cons_small", 0, 0);

	build_anim = App->anim->GetAnimation("boathouse");
	cons_anim = App->anim->GetAnimation("cons_small");

	tile = App->map->WorldToMap(x, y);
	position = App->map->MapToWorld<fPoint>(tile.x, tile.y);

	max_health = 500;
	if (built_state != BUILDING)
	{
		health = max_health;
		(*App->pathfinding->WorldToNode(tile.x, tile.y))->built = true;
	}

	rect = build_anim.GetCurrentFrame();

	load = { 0, 0, 0, 1000 };
}

j1BoatHouse::~j1BoatHouse()
{
}

void j1BoatHouse::Update(float dt)
{
	if (selected)
	{
		if (App->godmode)
		{
			if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
				BuildUnit(EntityType::BOAT, 1);
		}
	}

	App->render->AddBlitEvent(1, texture, GetRenderPositionX(), GetRenderPositionY(), rect);

	if (health == 0)
		CleanUp();
}



void j1BoatHouse::CleanUp()
{
	to_delete = true;
}
