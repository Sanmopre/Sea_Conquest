#include "j1Entities.h"

#include "j1Render.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "j1ParticleManager.h"
#include "j1InGameUI.h"
#include "j1Window.h"

#include <vector>

j1Harvester::j1Harvester(float x, float y, int level, int team)
{
	type = EntityType::HARVESTER;
	position.x = x;
	position.y = y;
	destination = position;
	this->level = level;
	trading_range = 50;
	this->team = team;
	speed = 30;
	range = 50;
	power = 15;
	harvestrate = { 2 };
	max_health = 50;
	health = max_health;
	storage = { 0, 0, 0, 500 };
	target = nullptr;

	automatic = false;
	automating = false;

	for (std::vector<Animation>::iterator i = App->entitymanager->allAnimations.begin(); i != App->entitymanager->allAnimations.end(); i++)
	{
		if (strcmp("entity_eight_north", i->name) == 0)
			north = i->GetAnimation();
		if (strcmp("entity_eight_northeast", i->name) == 0)
			north_east = i->GetAnimation();
		if (strcmp("entity_eight_east", i->name) == 0)
			east = i->GetAnimation();
		if (strcmp("entity_eight_southeast", i->name) == 0)
			south_east = i->GetAnimation();
		if (strcmp("entity_eight_south", i->name) == 0)
			south = i->GetAnimation();
		if (strcmp("entity_eight_southwest", i->name) == 0)
			south_west = i->GetAnimation();
		if (strcmp("entity_eight_west", i->name) == 0)
			west = i->GetAnimation();
		if (strcmp("entity_eight_northwest", i->name) == 0)
			north_west = i->GetAnimation();
	}

	for (std::vector<TextureInfo>::iterator e = App->entitymanager->allTextures.begin(); e != App->entitymanager->allTextures.end(); e++)
		if (type == e->type && level == e->level)
		{
			texture = e->texture;
			break;
		}

	rect = north.GetCurrentFrame();
}

j1Harvester::~j1Harvester()
{

}

void j1Harvester::Update(float dt)
{
	if (dt != 0.0f)
	{
		showing_hpbar = false;

		if (selected)
		{
			if (team == 1)
			{
				if(!automating)
					if (App->input->GetMouseButtonDown(3) == KEY_DOWN)
					{
						SetDestination(NodeType::ALL);
						automatic = false;
					}

				if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
				{
					SetAutomatic();
				}

				if (this == App->InGameUI->selected)
					Trading();
			}

			ShowHPbar(10, 5);
		}

		if (automating && harvest_destination == position)
			if (App->input->GetMouseButtonDown(3) == KEY_DOWN)
				if (FindTarget(range, EntityType::STORAGE) != nullptr)
				{
					iPoint m;
					App->input->GetMousePosition(m.x, m.y);
					m.x -= App->render->camera.x / App->win->GetScale();
					m.y -= App->render->camera.y / App->win->GetScale();
					deposit_destination = { (float)m.x, (float)m.y };
					automating = false;
					automatic = true;
				}
				else
				{
					automating = false;
					harvest_destination = {};
				}
		else if (automating && deposit_destination == position)
			if (App->input->GetMouseButtonDown(3) == KEY_DOWN)
				if (FindTarget(range, EntityType::RESOURCE) != nullptr)
				{
					iPoint m;
					App->input->GetMousePosition(m.x, m.y);
					m.x -= App->render->camera.x / App->win->GetScale();
					m.y -= App->render->camera.y / App->win->GetScale();
					harvest_destination = { (float)m.x, (float)m.y };
					automating = false;
					automatic = true;
				}
				else
				{
					automating = false;
					deposit_destination = {};
				}

		if (automatic)
		{
			if (harvest_destination == position)
			{
				if (storage.Total() == storage.maxweight || target->storage.Total() == 0)
					GoTo(deposit_destination, NodeType::ALL);
				else
				{
					target = FindTarget(range, EntityType::RESOURCE);
				}
			}
			else if (deposit_destination == position)
			{
				if (storage.Total() == 0)
					GoTo(harvest_destination, NodeType::ALL);
				else
				{
					target = FindTarget(range, EntityType::STORAGE);
				}
			}
		}
		else
			if (storage.Total() != storage.maxweight)
				target = FindTarget(range, EntityType::RESOURCE);

		if (destination != position)
			Move(dt);
		else
		{
			NextStep();

			if (target != nullptr)
			{
				if (target->type == EntityType::RESOURCE)
				{
					harvestrate.counter += dt;
					if (harvestrate.counter >= harvestrate.iterations)
					{
						Harvest(power, target);
						harvestrate.counter = 0;
					}
				}
				else if (target->type == EntityType::STORAGE)
				{
					//TRANFER RESOURCES
				}
			}
		}
		
		SelectAnimation();
	}

	App->render->AddBlitEvent(1, texture, GetRenderPositionX(), GetRenderPositionY(), rect);
	
	if (health == 0)
		CleanUp();
}

void j1Harvester::CleanUp()
{
	path.erase(path.begin(), path.end());
	path.shrink_to_fit();
	to_delete = true;
}

void j1Harvester::SetAutomatic()
{
	if (!automatic)
	{
		automating = true;
		//if (FindTarget(range, EntityType::RESOURCE) != nullptr)
			harvest_destination = position;
		//else if (FindTarget(range, EntityType::STORAGE) != nullptr)
		//	deposit_destination = position;
		//else
		//	automating = false;
	}
	else
		automatic = false;
}

void j1Harvester::Harvest(int power, j1Entity* target)
{
	if (target->storage.wood != 0)
	{
		if (target->storage.wood < power)
		{
			int left = target->storage.wood;
			target->storage.wood -= left;
			storage.wood += left;
		}
		else
		{
			target->storage.wood -= power;
			storage.wood += power;
		}
		if (storage.wood > storage.maxweight)
		{
			int left = storage.wood - storage.maxweight;
			storage.wood -= left;
			target->storage.wood += left;
		}
	}
	else if (target->storage.cotton != 0)
	{
		if (target->storage.cotton < power)
		{
			int left = target->storage.cotton;
			target->storage.cotton -= left;
			storage.cotton += left;
		}
		else
		{
			target->storage.cotton -= power;
			storage.cotton += power;
		}
		if (storage.cotton > storage.maxweight)
		{
			int left = storage.cotton - storage.maxweight;
			storage.cotton -= left;
			target->storage.cotton += left;
		}
	}
	else if (target->storage.metal != 0)
	{
		if (target->storage.metal < power)
		{
			int left = target->storage.metal;
			target->storage.metal -= left;
			storage.metal += left;
		}
		else
		{
			target->storage.metal -= power;
			storage.metal += power;
		}
		if (storage.metal > storage.maxweight)
		{
			int left = storage.metal - storage.maxweight;
			storage.metal -= left;
			target->storage.metal += left;
		}
	}
}