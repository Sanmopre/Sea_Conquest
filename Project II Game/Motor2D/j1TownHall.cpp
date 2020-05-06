#include "j1Entities.h"
#include "j1Render.h"

#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1Scene.h"

j1TownHall::j1TownHall(float x, float y, int team)
{
	tile = App->map->WorldToMap(x, y);
	position = App->map->MapToWorld<fPoint>(tile.x, tile.y);

	rect = { 64, 0, 64, 64 };
	built_rect = rect; /////////////////////
	this->team = team;
	load = { 0,0,0, 100 };

	max_health = 250;
	health = max_health;
	(*App->pathfinding->WorldToNode(tile.x, tile.y))->built = true;

	type = EntityType::TOWNHALL;
	level = 1;

	texture = App->tex->GetTexture("buildings", level, team);
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

	App->render->AddBlitEvent(1, texture, GetRenderPositionX(), GetRenderPositionY(), rect);
}

void j1TownHall::CleanUp()
{
	to_delete = true;
}