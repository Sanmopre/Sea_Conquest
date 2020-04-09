#include "j1Entities.h"
#include "j1Render.h"

j1Resource::j1Resource(float x, float y, int level)
{
	this->type = EntityType::RESOURCE;
	main_type = EntityType::RESOURCE;
	position.x = x;
	position.y = y;
	this->level = level;
	team = 0;
	max_health = 1000;
	health = max_health;

	load.maxweight = 100000 * level;
	load.wood = load.maxweight / WOOD_MASS;
	load.cotton = 0;
	load.metal = 0;

	rect.w = 10;
	rect.h = 10;
}

void j1Resource::Update(float dt)
{
	showing_hpbar = false;

	if (health == 0)
		CleanUp();

	if (selected)
		ShowHPbar(10, 5);

	rect.x = GetRenderPositionX();
	rect.y = GetRenderPositionY();
	App->render->AddBlitEvent(0, nullptr, 0, 0, rect, false, false, 100, 100, 100, 255);
}

void j1Resource::CleanUp()
{
	to_delete = true;
}