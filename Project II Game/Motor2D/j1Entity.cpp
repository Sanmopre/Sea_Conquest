#include "j1Entities.h"

#include "j1Render.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include <vector>

void j1Entity::Damage(int damage, j1Entity* target)
{
	if (target != nullptr)
	{
		target->health -= damage;
		if (target->health < 0)
			target->health = 0;
	}
}

void  j1Entity::ShowHPbar(int extra_width, int height)
{
	if (!showing_hpbar)
	{
		showing_hpbar = true;

		SDL_Rect Brect = { position.x - extra_width, position.y - 20, rect.w + extra_width * 2, height };
		Color Bcolor(96u, 96u, 96u);

		float max_w = Brect.w;
		float width = max_w;

		App->render->AddBlitEvent(2, nullptr, 0, 0, Brect, false, false, Bcolor.r, Bcolor.g, Bcolor.b, Bcolor.a); //health grey bar

		width /= max_health;
		width *= health;
		Brect.w = width;
		Bcolor.SetColor(0u, 204u, 0u);
		App->render->AddBlitEvent(2, nullptr, 0, 0, Brect, false, false, Bcolor.r, Bcolor.g, Bcolor.b, Bcolor.a); //health bar

		Brect.y += 10;

		width = max_w;
		width /= storage.maxweight;
		width *= storage.wood;
		Brect.w = width;
		Bcolor.SetColor(120u, 72u, 0u);
		App->render->AddBlitEvent(2, nullptr, 0, 0, Brect, false, false, Bcolor.r, Bcolor.g, Bcolor.b, Bcolor.a); // wood bar

		Brect.x += Brect.w;
		width = max_w;
		width /= storage.maxweight;
		width *= storage.cotton;
		Brect.w = width;
		Bcolor.SetColor(240u, 240u, 240u);
		App->render->AddBlitEvent(2, nullptr, 0, 0, Brect, false, false, Bcolor.r, Bcolor.g, Bcolor.b, Bcolor.a); // cotton bar

		Brect.x += Brect.w;
		width = max_w;
		width /= storage.maxweight;
		width *= storage.metal;
		Brect.w = width;
		Bcolor.SetColor(107u, 120u, 119u);
		App->render->AddBlitEvent(2, nullptr, 0, 0, Brect, false, false, Bcolor.r, Bcolor.g, Bcolor.b, Bcolor.a); // metal bar
	}
}

void j1Entity::Trading()
{
	for (std::vector<j1Entity*>::iterator e = App->entitymanager->entities.begin(); e != App->entitymanager->entities.end(); e++)
	{
		if (*e != this && team == (*e)->team)
			if (position.x + trading_range > (*e)->position.x &&
				position.x - trading_range < (*e)->position.x &&
				position.y + trading_range >(*e)->position.y &&
				position.y - trading_range < (*e)->position.y)
			{
				bool found = false;
				for (std::vector<j1Entity*>::iterator t = tradeable_list.begin(); t != tradeable_list.end(); t++)
				{
					if (*e == *t)
					{
						found = true;
						break;
					}
				}
				if (!found)
					tradeable_list.push_back(*e);
			}
			else
			{
				for (std::vector<j1Entity*>::iterator t = tradeable_list.begin(); t != tradeable_list.end(); t++)
				{
					if (*e == *t)
					{
						tradeable_list.erase(t);
						break;
					}
				}
			}
	}

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
		trading_entity_offset--;

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		trading_entity_offset++;
	
	if (trading_entity_offset < 0)
		trading_entity_offset = tradeable_list.size() - 1;
	if (trading_entity_offset > tradeable_list.size() - 1)
		trading_entity_offset = 0;

	int counter = 0;

	while (counter != tradeable_list.size())
	{
		std::vector<j1Entity*>::iterator t = tradeable_list.begin();
		t += counter;
		for (; t != tradeable_list.end(); t++)
		{
			if ((*t)->health == 0)
			{
				tradeable_list.erase(t);
				tradeable_list.shrink_to_fit();
				break;
			}
			else if (t == tradeable_list.begin() + trading_entity_offset)
			{
				trading_entity = *t;
				App->render->AddBlitEvent(0, nullptr, 0, 0, { (int)(*t)->position.x, (int)(*t)->position.y, 30, 30 }, false, false, 255, 255, 0, 100);
			}

			counter++;
		}
	}
}