#include "j1Entities.h"

#include "j1Input.h"
#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1InGameUI.h"
#include "j1Scene.h"

j1Structure::j1Structure()
{ 
	main_type = EntityType::STRUCTURE;
	
	placed = true;

	if (App->scene->start)
		built_state = NOT_BUILDING;
	else
	{
		built_state = BUILDING;
		health = 1;
		App->audio->PlayFx(App->audio->structure_build);
	}
		

	other_rect = {192, 0, 63, 63}; //////////////
}


j1Structure::~j1Structure()
{
	App->audio->PlayFx(App->audio->structure_destroy);
	unitqueue.erase(unitqueue.begin(), unitqueue.end());
	unitqueue.shrink_to_fit();
}

void j1Structure::Primitive_Update(float dt) 
{
	showing_hpbar = false;

	if (built_state == BUILDING)
	{
		ShowHPbar(10, 5);
	}
	else
	{
		NotPlacedBehaviour();

		if (App->InGameUI->selected != nullptr)
			if (this == App->InGameUI->selected->trading_entity)
				ShowHPbar(10, 5);

		if (selected)
		{
			ShowHPbar(10, 5);

			if (this == App->InGameUI->selected)
				Trading();

			if (App->godmode)
			{
				if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
					placed = false;
			}
		}

		BuildProcces(dt);
	}

		if (built_state == BUILDING || built_state == ON_HOLD)
			rect = other_rect;
		else
			rect = built_rect;
}

void j1Structure::NotPlacedBehaviour()
{
	if (!placed)
	{
		position = App->input->GetMouseWorldPosition();

		iPoint placing_tile = App->map->WorldToMap(position.x, position.y);
		position = App->map->MapToWorld<fPoint>(placing_tile.x, placing_tile.y);
		if (App->godmode)
		{
			if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
				CleanUp();
		}
	}
}

void j1Structure::BuildUnit(EntityType type, int level)
{
	EntityRequest unit(position.x + 50, position.y + 20, type, level, team);
	unitqueue.push_back(unit);
}

void j1Structure::BuildProcces(float dt)
{
	if (unitqueue.size() != 0)
	{
		if (unitqueue.begin()->type == EntityType::BOAT)
			building_time.iterations = 5 * level / 2;
		if (unitqueue.begin()->type == EntityType::HARVESTER)
			building_time.iterations = 8 * level / 2;

		building_time.counter += dt;
		if (building_time.counter >= building_time.iterations)
		{
			for (std::vector<j1Entity*>::iterator e = App->entitymanager->entities.begin(); e != App->entitymanager->entities.end(); e++)
			{
				bool changed = true;
				while (changed)
				{
					changed = false;
					if ((*e)->position.x == unitqueue.begin()->x && (*e)->position.y == unitqueue.begin()->y)
					{
						unitqueue.begin()->x += 20;
						unitqueue.begin()->y += 20;
						changed = true;
					}
				}
			}
			App->entitymanager->AddEntity(unitqueue.begin()->x, unitqueue.begin()->y, unitqueue.begin()->type, unitqueue.begin()->level, unitqueue.begin()->team);
			unitqueue.erase(unitqueue.begin());
			if (unitqueue.size() <= unitqueue.capacity() / 2)
				unitqueue.shrink_to_fit();
			building_time.counter = 0;
		}
	}
}