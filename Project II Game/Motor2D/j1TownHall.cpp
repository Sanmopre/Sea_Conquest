#include "j1Entities.h"
#include "j1Render.h"

#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1Scene.h"
#include "j1Minimap.h"

j1TownHall::j1TownHall(float x, float y, int team)
{
	tile = App->map->WorldToMap(x, y);
	position = App->map->MapToWorld<fPoint>(tile.x, tile.y);
	level = 1;
	this->team = team;
	fog_range = 5;

	texture = App->tex->GetTexture("townhall", level, team);
	tex_construction = App->tex->GetTexture("cons_medium", 0, 0);

	basic = App->anim->GetAnimation("townhall");

	under_construction = App->anim->GetAnimation("cons_medium");

	
	load = { 0,0,0, 0 };

	max_health = 250;
	health = max_health;
	(*App->pathfinding->WorldToNode(tile.x, tile.y))->built = true;

	type = EntityType::TOWNHALL;

	current_animation = &basic;
}

j1TownHall::~j1TownHall()
{

}

void j1TownHall::Update(float)
{
	if (health == 0)
		if (team == 1)
			App->scene->state = LOSE;
		else if (team == 2)
			App->scene->state = WIN;

	if (App->fog->GetVisibility(tile.x, tile.y) == FogState::VISIBLE || App->ignore_fog)
	{
		App->render->AddBlitEvent(1, texture, GetRenderPositionX(), GetRenderPositionY(), rect);
		App->minimap->Draw_entities(this);
	}
}

void j1TownHall::CleanUp()
{
	to_delete = true;
}