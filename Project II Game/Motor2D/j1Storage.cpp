#include "j1Entities.h"
#include "j1Render.h"

#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1InGameUI.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Minimap.h"

j1Storage::j1Storage(float x, float y, int team)
{

	tile = App->map->WorldToMap(x, y);
	position = App->map->MapToWorld<fPoint>(tile.x, tile.y);
	terrain = NodeType::GROUND;
	level = 1;
	this->team = team;
	fog_range = 3;

	texture = App->tex->GetTexture("storage", level, team);
	tex_construction = App->tex->GetTexture("cons_small", 0, 0);

	basic = App->anim->GetAnimation("ally_storage_empty");
	low = App->anim->GetAnimation("ally_storage_low");
	half = App->anim->GetAnimation("ally_storage_half");
	full = App->anim->GetAnimation("ally_storage_full");
	under_construction = App->anim->GetAnimation("cons_small");

	
	load = { 0,0,0,2000 };
	int range = 100;
	trading_range = 200;
	max_health = 250;
	if (built_state != BUILDING)
	{
		health = max_health;
		(*App->pathfinding->WorldToNode(tile.x, tile.y))->built = true;
		load.metal += 30;
	}
	type = EntityType::STORAGE;
}

j1Storage::~j1Storage()
{

}

void j1Storage::Update(float dt)
{
	bool flip = false;
	if (team == 1)
	{
		if(built_state == NOT_BUILDING)
			SearchStructures();

		flip = true;
		if (selected)
		{

			if (App->godmode)
			{
				if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
					BuildUnit(EntityType::HARVESTER, 1);
			}
		}
	}

	current_animation = &basic;

	float percent = 100.0f / load.maxweight;
	percent *= load.Weight();

	if (percent > 0 && percent < 50)
		current_animation = &low;
	else if (percent >= 50 && percent < 100)
		current_animation = &half;
	else if (percent >= 100)
		current_animation = &full;
	
	App->render->AddBlitEvent(0, nullptr, 1, 0, { (int)position.x,(int)position.y + 16, trading_range, 0 }, false, false, 100, 100, 100, 150);
	iPoint p = App->map->WorldToMap(position.x, position.y);
	FogState s = App->fog->GetVisibility(p.x, p.y);
	if (s == FogState::VISIBLE || App->ignore_fog)
	{
		App->render->AddBlitEvent(1, current_tex, GetRenderPositionX(), GetRenderPositionY(), rect, flip);
		App->minimap->Draw_entities(this);
	}
}

void j1Storage::CleanUp()
{
	to_delete = true;
}

void j1Storage::SearchStructures()
{
	int i = 1;
	j1Entity* entity;

	bool begining = false;
	bool end = false;

	bool stop = false;

	float distance;
	float shortest = trading_range;
	while (!stop)
	{
		if (spot + i == App->entitymanager->entities.begin() - 1)
			begining = true;
		else if (spot + i == App->entitymanager->entities.end())
			end = true;

		if (begining && end)
		{
			stop = true;
		}
		else
		{
			if ((begining && i < 0) || (end && i > 0))
			{
			}
			else
			{
				bool skip = false;
				entity = *(spot + i);

				if ((entity->position.y > position.y + trading_range || entity->position.y < position.y - trading_range) ||
					(entity->position.x > position.x + trading_range || entity->position.x < position.x - trading_range))
				{
					if ((entity->position.y > position.y + trading_range || entity->position.y < position.y - trading_range) &&
						(entity->position.x > position.x + trading_range || entity->position.x < position.x - trading_range))
					{
						if (i < 0)
							begining = true;
						else if (i > 0)
							end = true;
					}
					else
						skip = true;
				}

				if (!stop)
				{
					vector<j1Entity*>* storages = entity->GetStorages();
					if (storages != nullptr)
					{
						for(vector<j1Entity*>::iterator itr = storages->begin(); itr != storages->end(); itr++)
							if (*itr == this)
							{
								storages->erase(itr);
								break;
							}

						if (team == entity->team && !skip)
						{
							if (App->entitymanager->InsideElipse(position, entity->position, trading_range))
							{
								storages->push_back(this);
							}
						}
					}
				}
			}
			if (i > 0)
				i *= -1;
			else
			{
				i *= -1;
				i++;
			}
		}
	}
}