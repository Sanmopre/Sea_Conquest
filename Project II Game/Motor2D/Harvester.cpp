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
	load = { 0, 0, 0, 30 };
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
		if(automating)
			if (App->input->GetMouseButtonDown(3) == KEY_DOWN)
				if (harvest_destination == position)
				{
					iPoint m;
					App->input->GetMousePosition(m.x, m.y);
					m.x -= App->render->camera.x / App->win->GetScale();
					m.y -= App->render->camera.y / App->win->GetScale();
					deposit_destination = { (float)m.x, (float)m.y };
					automating = false;
					automatic = true;
				}
				else if (deposit_destination == position)
				{
					iPoint m;
					App->input->GetMousePosition(m.x, m.y);
					m.x -= App->render->camera.x / App->win->GetScale();
					m.y -= App->render->camera.y / App->win->GetScale();
					harvest_destination = { (float)m.x, (float)m.y };
					automating = false;
					automatic = true;
				}

		if (automatic)
		{
			if (harvest_destination == position)
			{
				LOG("Total: %d, Max: %d", load.Total(), load.maxweight);
				if (load.Total() == load.maxweight || target->load.Total() == 0)
					GoTo(deposit_destination, NodeType::ALL);
				else
				{
					target = FindTarget(range, EntityType::RESOURCE, -1);
				}
			}
			else if (deposit_destination == position)
			{
				if (load.Total() == 0)
					GoTo(harvest_destination, NodeType::ALL);
				else
				{
					target = FindTarget(range, EntityType::STORAGE, team);
				}
			}
		}
		else
			if (load.Total() != load.maxweight)
				target = FindTarget(range, EntityType::RESOURCE, -1);

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
		if (FindTarget(range, EntityType::RESOURCE, -1) != nullptr)
			harvest_destination = position;
		else if (FindTarget(range, EntityType::STORAGE, team) != nullptr)
			deposit_destination = position;
		else
			automating = false;
	}
	else
		automatic = false;
}

void j1Harvester::Harvest(int power, j1Entity* target)
{
	if (target->load.wood != 0)
	{
		if (target->load.wood < power)
		{
			int left = target->load.wood;
			target->load.wood -= left;
			load.wood += left;
		}
		else
		{
			target->load.wood -= power;
			load.wood += power;
		}
		if (load.wood > load.maxweight)
		{
			int left = load.wood - load.maxweight;
			load.wood -= left;
			target->load.wood += left;
		}
	}
	else if (target->load.cotton != 0)
	{
		if (target->load.cotton < power)
		{
			int left = target->load.cotton;
			target->load.cotton -= left;
			load.cotton += left;
		}
		else
		{
			target->load.cotton -= power;
			load.cotton += power;
		}
		if (load.cotton > load.maxweight)
		{
			int left = load.cotton - load.maxweight;
			load.cotton -= left;
			target->load.cotton += left;
		}
	}
	else if (target->load.metal != 0)
	{
		if (target->load.metal < power)
		{
			int left = target->load.metal;
			target->load.metal -= left;
			load.metal += left;
		}
		else
		{
			target->load.metal -= power;
			load.metal += power;
		}
		if (load.metal > load.maxweight)
		{
			int left = load.metal - load.maxweight;
			load.metal -= left;
			target->load.metal += left;
		}
	}
}