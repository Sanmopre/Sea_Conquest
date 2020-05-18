#include "j1Entities.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Minimap.h"

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

	anim = App->anim->GetAnimation("resource");
	rect = anim.GetCurrentFrame();

	load.maxweight = 1000 * level;

	switch (type)
	{
	case EntityType::ALL_COTTON:
		load.wood = 0;
		load.cotton = load.maxweight / COTTON_MASS;
		load.metal = 0;
		color.SetColor(240u, 240u, 240u);
		texture = App->tex->GetTexture("cotton", level, 0);
		break;
	case EntityType::ALL_WOOD:
		load.wood = load.maxweight / WOOD_MASS;
		load.cotton = 0;
		load.metal = 0;
		color.SetColor(120u, 72u, 0u);
		texture = App->tex->GetTexture("wood", level, 0);
		break;
	case EntityType::ALL_METAL:
		load.wood = 0;
		load.cotton = 0;
		load.metal = load.maxweight / METAL_MASS;
		color.SetColor(107u, 120u, 119u);
		texture = App->tex->GetTexture("metal", level, 0);
		break;
	}
}

void j1Resource::Update(float dt)
{
	showing_hpbar = false;

	if (selected)
		ShowHPbar(0, 5, -20);

	if (load.Total() == 0)
		color.Black();

	selectable_area = rect;
	selectable_area.x = GetRenderPositionX();
	selectable_area.y = GetRenderPositionY();
	selectable_area.h /= 2;
	selectable_area.y += selectable_area.h;

	if (App->fog->GetVisibility(position) == FogState::VISIBLE || App->ignore_fog)
	{
		App->render->AddBlitEvent(1, texture, GetRenderPositionX(), GetRenderPositionY(), rect);
		App->minimap->Draw_entities(this);
	}
}

void j1Resource::CleanUp()
{
	to_delete = true;
}