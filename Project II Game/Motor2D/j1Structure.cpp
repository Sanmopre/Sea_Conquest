#include "j1Entities.h"

#include "j1Map.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "j1InGameUI.h"
#include "j1Scene.h"
#include "j1Pathfinding.h"
#include "j1ParticleManager.h"

j1Structure::j1Structure()
{ 
	main_type = EntityType::STRUCTURE;
	
	placed = true;

	if (App->scene->start || App->godmode)
		built_state = NOT_BUILDING;
	else
	{
		built_state = BUILDING;
		health = 1;
	}
		
	other_rect = {192, 0, 63, 63}; //////////////
}


j1Structure::~j1Structure()
{
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

	if(health <= 0)
		App->audio->PlaySpatialFx(App->audio->structure_destroy,
			App->audio->GetAngle(App->render->getCameraPosition(), { (int)position.x, (int)position.y }),
			App->audio->GetDistance(App->render->getCameraPosition(), { (int)position.x, (int)position.y }));
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
			{
				CleanUp();
				App->audio->PlaySpatialFx(App->audio->structure_destroy,
					App->audio->GetAngle(App->render->getCameraPosition(), { (int)position.x, (int)position.y }),
					App->audio->GetDistance(App->render->getCameraPosition(), { (int)position.x, (int)position.y }));
			}
		}
	}
}

void j1Structure::BuildUnit(EntityType type, int level)
{
	iPoint p;
	fPoint pos;
	int limit = 4;
	int r = 64 * limit;
	bool out = false;
	for (int i = 1; i < limit; i++)
	{
		int line = 3 * i - (i - 1);
		int amount = line + 2 * line - 2 + line - 2;
		iPoint s = { tile.x - line / 2,tile.y - line / 2 };
		p = s;
		for (int y = 0; y < amount; y++)
		{
			if (App->pathfinding->PointToNode(p.x, p.y, &App->pathfinding->NodeMap)->type == terrain)
			{
				out = true;
				fPoint a = App->map->MapToWorld<fPoint>(p.x, p.y);
				for (vector<j1Entity*>::iterator e = App->entitymanager->entities.begin(); e != App->entitymanager->entities.end(); e++)
				{
					if ((*e)->main_type == EntityType::UNIT)
							if ((*e)->position == a)
								out = false;
				}
				for (vector<EntityRequest>::iterator e = unitqueue.begin(); e != unitqueue.end(); e++)
				{
					fPoint b = { e->x, e->y };
					if (b == a)
						out = false;
				}
				
				if (out)
					break;
			}
			if (y < line - 1)
				p.x += 1;
			if(y >= amount - line)
				p.x += 1;
			if (y >= line && y < amount - line)
				if (y % 2 == 0)
				{
					p.x = s.x;
					p.y += 1;
				}
				else
				{
					p.x = s.x + line - 1;
					p.y += 1;
				}
		}
		if (out)
		{
			pos = App->map->MapToWorld<fPoint>(p.x, p.y);
			EntityRequest unit(pos.x, pos.y, type, level, team);
			unitqueue.push_back(unit);
			break;
		}
	}
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

		SDL_Rect Brect = { GetRenderPositionX(), GetRenderPositionY() + rect.h, rect.w, 5 };
		Color Bcolor(96u, 96u, 96u);

		float max_w = Brect.w;
		float width = max_w;

		App->render->AddBlitEvent(2, nullptr, 0, 0, Brect, false, false, Bcolor.r, Bcolor.g, Bcolor.b, Bcolor.a); //health grey bar

		width /= building_time.iterations;
		width *= building_time.counter;
		Brect.w = width;
		Bcolor.SetColor(255u, 100u, 0u);
		App->render->AddBlitEvent(2, nullptr, 0, 0, Brect, false, false, Bcolor.r, Bcolor.g, Bcolor.b, Bcolor.a); //health bar

		if (building_time.counter >= building_time.iterations)
		{
			App->entitymanager->AddEntity(unitqueue.begin()->x, unitqueue.begin()->y, unitqueue.begin()->type, unitqueue.begin()->level, unitqueue.begin()->team);
			unitqueue.erase(unitqueue.begin());
			if (unitqueue.size() <= unitqueue.capacity() / 2)
				unitqueue.shrink_to_fit();
			building_time.counter = 0;
		}
	}
}