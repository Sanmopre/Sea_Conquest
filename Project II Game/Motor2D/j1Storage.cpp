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

	texture = App->tex->GetTexture("storage", level, this->team);
	cons_tex = App->tex->GetTexture("cons_medium", 0, 0);

	build_anim = App->anim->GetAnimation("storage");
	cons_anim = App->anim->GetAnimation("cons_medium");

	
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


	rect = build_anim.GetCurrentFrame();
}

j1Storage::~j1Storage()
{

}

void j1Storage::Update(float dt)
{
	if(team == 1)
		if (selected)
		{
			if (App->godmode)
			{
				if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
					BuildUnit(EntityType::HARVESTER, 1);
			}
		}

	App->render->AddBlitEvent(1, texture, GetRenderPositionX(), GetRenderPositionY(), rect);
}

void j1Storage::CleanUp()
{
	to_delete = true;
}