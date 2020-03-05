#include "j1Entities.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Input.h"

#include "j1EntityManager.h"
#include "j1Map.h"

BoatHouse::BoatHouse(int team, iPoint tile)
{
	type = Entity_Type::BOATHOUSE;
	position = App->input->GetMouseWorldPosition();
	level = 1;
	this->team = team;
	max_health = 500;
	health = max_health;
	rect = { (int)position.x, (int)position.y, 40, 40 };
}

BoatHouse::~BoatHouse()
{

}

void BoatHouse::Update(float dt)
{
	showing_hpbar = false;
	rect.x = position.x;
	rect.y = position.y;

	NotPlacedBehaviour();

	if (selected)
	{
		if (team == 0)
			color.SetColor(0u, 255u, 255u);
		else if (team == 1)
			color.SetColor(255u, 255u, 0u);

		if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
			placed = false;

		if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
			BuildUnit(Entity_Type::BOAT, 1);
		
		ShowHPbar(10, 5);
	}
	else
		if (team == 0)
			color.Blue();
		else if (team == 1)
			color.Red();

	if (unitqueue.size() != 0)
	{
		if (unitqueue.begin()->type == Entity_Type::BOAT)
			building_time.iterations = 5;

		building_time.counter += dt;
		if (building_time.counter >= building_time.iterations)
		{
			for (std::vector<Entity*>::iterator e = App->entitymanager->entities.begin(); e != App->entitymanager->entities.end(); e++)
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
			App->entitymanager->RequestEntity(*unitqueue.begin());
			unitqueue.erase(unitqueue.begin());
			unitqueue.shrink_to_fit();
			building_time.counter = 0;
		}
	}

	App->render->AddBlitEvent(1, nullptr, 0, 0, rect, false, false, color.r, color.g, color.b, color.a);

	if (health == 0)
		CleanUp();
}

void BoatHouse::BuildUnit(Entity_Type type, int level)
{
	EntityRequest unit = { position.x + 50, position.y + 20, type, level, team };
	unitqueue.push_back(unit);
}

void BoatHouse::CleanUp()
{
	unitqueue.erase(unitqueue.begin(), unitqueue.end());
	unitqueue.shrink_to_fit();
	to_delete = true;
}
