#include "j1Entities.h"
#include "j1Render.h"

#include "j1Map.h"
#include "j1Scene.h"
#include "j1InGameUI.h"

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

	rect = { (int)position.x, (int)position.y, 30, 30 };
	this->team = team;
	load = { 0,0,0,2000 };
	int range = 100;
	trading_range = 200;
	max_health = 250;
	health = max_health;
	
	type = EntityType::STORAGE;
	level = 1;
}

j1Storage::~j1Storage()
{

}

void j1Storage::Update(float)
{
	showing_hpbar = false;

	NotPlacedBehaviour();

	if(team == 1)
		if (selected)
		{
			ShowHPbar(10, 5);

			if (this == App->InGameUI->selected)
				Trading();
		}

	App->render->AddBlitEvent(1, nullptr, 0, 0, rect, false, false, 100, 0, 255);
}

void j1Storage::CleanUp()
{
	to_delete = true;
}