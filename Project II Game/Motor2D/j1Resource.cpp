#include "j1Entities.h"
#include "j1Render.h"
#include "j1Map.h"

j1Resource::j1Resource(float x, float y, int level,EntityType type)
{
	this->type = type;
	main_type = EntityType::RESOURCE;

	iPoint tile = App->map->WorldToMap(x, y);
	position = App->map->MapToWorld<fPoint>(tile.x, tile.y);

	this->level = level;
	team = 0;
	max_health = 1000;
	health = max_health;


	load.maxweight = 5000 * level;

	switch (type)
	{
	case EntityType::ALL_COTTON:
		load.wood = 0;
		load.cotton = load.maxweight / COTTON_MASS;
		load.metal = 0;
		color.SetColor(240u, 240u, 240u);
		break;
	case EntityType::ALL_WOOD:
		load.wood = load.maxweight / WOOD_MASS;
		load.cotton = 0;
		load.metal = 0;
		color.SetColor(120u, 72u, 0u);
		break;
	case EntityType::ALL_METAL:
		load.wood = 0;
		load.cotton = 0;
		load.metal = load.maxweight / METAL_MASS;
		color.SetColor(107u, 120u, 119u);
		break;
	}

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

	selectable_area = rect;
	if (App->fog->GetVisibility(position) == FogState::VISIBLE)
		App->render->AddBlitEvent(0, nullptr, 0, 0, rect, false, false, color.r, color.g, color.b, 255);
}

void j1Resource::CleanUp()
{
	to_delete = true;
}