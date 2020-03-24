#include "j1Entities.h"
#include "j1Render.h"

j1Resource::j1Resource(float x, float y, int level, EntityType type)
{
	position.x = x;
	position.y = y;
	this->level = level;
	this->type = type;

	health = 1000;
	storage.maxweight = 1000 * level;

	switch (type)
	{
	case EntityType::WOOD_RESOURCE:
		storage.wood = storage.maxweight;
		storage.cotton = 0;
		storage.metal = 0;
		break;
	case EntityType::COTTON_RESOURCE:
		storage.wood = 0;
		storage.cotton = storage.maxweight;
		storage.metal = 0;
		break;
	case EntityType::METAL_RESOURCE:
		storage.wood = 0;
		storage.cotton = 0;
		storage.metal = storage.maxweight;
		break;
	case EntityType::EVEN2_RESOURCE:
		storage.wood = storage.maxweight / 2;
		storage.cotton = storage.maxweight / 2;
		storage.metal = 0;
		break;
	case EntityType::EVEN3_RESOURCE:
		storage.wood = storage.maxweight / 3;
		storage.cotton = storage.maxweight / 3;
		storage.metal = storage.maxweight / 3;
		break;
	case EntityType::NONE:
		storage.wood = 0;
		storage.cotton = 0;
		storage.metal = 0;
		break;
	}
}

void j1Resource::Update(float dt)
{
	if (health == 0)
		CleanUp();

	if (selected)
		ShowHPbar(10, 5);

	App->render->AddBlitEvent(0, nullptr, 0, 0, { GetRenderPositionX(), GetRenderPositionY(), 10,10 }, false, false, 100, 100, 100, 255);
}

void j1Resource::CleanUp()
{
	to_delete = true;
}