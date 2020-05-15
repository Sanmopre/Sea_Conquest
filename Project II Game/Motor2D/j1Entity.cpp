#include "j1Entities.h"

#include "j1Render.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "j1Textures.h"
#include <vector>

j1Entity::j1Entity() 
{ 
	selected = false;
	to_delete = false;
	to_remove = false;
	fog_range = 3;

	storages.push_back(this);
}
j1Entity::~j1Entity()
{
	trading_entity = nullptr;
	texture = nullptr;
	tradeable_list.erase(tradeable_list.begin(), tradeable_list.end());
	tradeable_list.shrink_to_fit();
	storages.erase(storages.begin(), storages.end());
	storages.shrink_to_fit();
}

void  j1Entity::ShowHPbar(int extra_width, int height, int distance)
{
	if (!showing_hpbar)
	{
		showing_hpbar = true;

		SDL_Rect Brect = { selectable_area.x - extra_width, selectable_area.y + distance, selectable_area.w + extra_width * 2, height };
		Color Bcolor(96u, 96u, 96u);

		float max_w = Brect.w;
		float width = max_w;

		App->render->AddBlitEvent(2, nullptr, 0, 0, Brect, false, false, Bcolor.r, Bcolor.g, Bcolor.b, Bcolor.a); //health grey bar

		width /= max_health;
		width *= health;
		Brect.w = width;
		Bcolor.SetColor(0u, 204u, 0u);
		App->render->AddBlitEvent(2, nullptr, 0, 0, Brect, false, false, Bcolor.r, Bcolor.g, Bcolor.b, Bcolor.a); //health bar

		if (load.maxweight != 0)
		{
			Brect.y += 10;

			width = max_w;
			width /= load.maxweight;
			width *= load.CottonWeight();
			Brect.w = width;
			Bcolor.SetColor(240u, 240u, 240u);
			App->render->AddBlitEvent(2, nullptr, 0, 0, Brect, false, false, Bcolor.r, Bcolor.g, Bcolor.b, Bcolor.a); // cotton bar

			Brect.x += Brect.w;
			width = max_w;
			width /= load.maxweight;
			width *= load.WoodWeight();
			Brect.w = width;
			Bcolor.SetColor(120u, 72u, 0u);
			App->render->AddBlitEvent(2, nullptr, 0, 0, Brect, false, false, Bcolor.r, Bcolor.g, Bcolor.b, Bcolor.a); // wood bar

			Brect.x += Brect.w;
			width = max_w;
			width /= load.maxweight;
			width *= load.MetalWeight();
			Brect.w = width;
			Bcolor.SetColor(107u, 120u, 119u);
			App->render->AddBlitEvent(2, nullptr, 0, 0, Brect, false, false, Bcolor.r, Bcolor.g, Bcolor.b, Bcolor.a); // metal bar
		}
	}
}

void j1Entity::Trading()
{
	tradeable_list.erase(tradeable_list.begin(), tradeable_list.end());

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
		trading_offset_modifier = -1;

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		trading_offset_modifier = 1;

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

				if (!stop && !skip)
				{
					if (team == entity->team && entity->selected && entity->load.maxweight != 0)
					{
						if (App->entitymanager->InsideElipse(position, entity->position, trading_range))
						{
							ShowHPbar(10, 5, -10);

							tradeable_list.push_back(entity);
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
	trading_total = tradeable_list.size();

	if(trading_total == 0)
		trading_entity = nullptr;

	int counter = 0;

	for (std::vector<j1Entity*>::iterator s = tradeable_list.begin(); s != tradeable_list.end(); s++)
	{
		if (*s == trading_entity)
		{
			trading_entity_offset = counter;
			break;
		}
		counter++;
	}

	trading_entity_offset += trading_offset_modifier;
	trading_offset_modifier = 0;

	if (trading_entity_offset < 0)
		trading_entity_offset = tradeable_list.size() - 1;
	if (trading_entity_offset > tradeable_list.size() - 1)
		trading_entity_offset = 0;

	counter = 0;

	while (counter != tradeable_list.size())
	{
		std::vector<j1Entity*>::iterator s = tradeable_list.begin();
		s += counter;
		for (; s != tradeable_list.end(); s++)
		{
			if ((*s)->health == 0.0f)
			{
				tradeable_list.erase(s);
				break;
			}
			else if (s == tradeable_list.begin() + trading_entity_offset)	
			{
				trading_entity = *s; 
				
				App->render->AddBlitEvent(0, nullptr, 0, 0, trading_entity->selectable_area, false, false, 0, 255, 255, 100);
			}

			counter++;
		}
	}
}

j1Entity* j1Entity::FindTarget(float x, float y, int range, EntityType type, EntityType main_type, int team)
{
	j1Entity* ret = nullptr;

	int i = 1;
	j1Entity* entity;

	bool begining = false;
	bool end = false;

	bool stop = false;

	float distance;
	float shortest = range;
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

				if ((entity->position.y > y + range || entity->position.y < y - range) ||
					(entity->position.x > x + range || entity->position.x < x - range))
				{
					if (i < 0 && (entity->position.y > y + range || entity->position.y < y - range) &&
						(entity->position.x > x + range || entity->position.x < x - range))
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
					if (this->terrain == NodeType::WATER && entity->terrain == NodeType::ALL)
					{
					}
					else
					if ((entity->team == team || team == 0) &&
						(entity->type == type || type == EntityType::NONE) &&
						(entity->main_type == main_type || main_type == EntityType::NONE)
						&& !skip)
					{
						if (App->entitymanager->InsideElipse({ x,y }, entity->position, range))
						{
							distance = sqrtf((x - entity->position.x) * (x - entity->position.x) + (y - entity->position.y) * (y - entity->position.y));
							if (distance < shortest)
							{
								shortest = distance;
								ret = entity;
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

	if(ret != nullptr)
		ret->ShowHPbar(10, 3, -10);
	return ret;
}

int j1Entity::GetRenderPositionX()
{
	return position.x - rect.w / 2;
}

int j1Entity::GetRenderPositionY()
{
	int center = 0;

	if (main_type == EntityType::UNIT)
		if (terrain == NodeType::ALL)
			center -= 48;
		else
			center = 16;

	return (position.y - rect.h / 2) + center;
}
