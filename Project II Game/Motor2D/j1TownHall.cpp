#include "j1Entities.h"
#include "j1Render.h"

#include "j1Map.h"
#include "j1Scene.h"

j1TownHall::j1TownHall(float x, float y, int team)
{
	placed = true;
	tile = App->map->WorldToMap(x, y);
	position = App->map->MapToWorld<fPoint>(tile.x, tile.y);

	rect = { (int)position.x, (int)position.y, 40, 40 };
	this->team = team;
	load = { 0,0,0, 100 };

	max_health = 250;
	health = max_health;

	type = EntityType::TOWNHALL;
	level = 1;
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
	showing_hpbar = false;

	if (selected)
		ShowHPbar(10, 5);

	App->render->AddBlitEvent(1, nullptr, 0, 0, rect, false, false, 100, 100, 255);
}

void j1TownHall::CleanUp()
{
	to_delete = true;
}