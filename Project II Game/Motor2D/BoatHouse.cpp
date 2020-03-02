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
	placed = false;
	rect = { position.x, position.y, 40, 40 };
}

BoatHouse::~BoatHouse()
{

}

void BoatHouse::Update(float dt)
{
	showing_hpbar = false;
	rect.x = position.x;
	rect.y = position.y;

	if (!placed)
	{
		if (team == 0)
			color.Blue();
		else if (team == 1)
			color.Red();

		position = App->input->GetMouseWorldPosition();

		iPoint placing_tile = App->map->WorldToMap(position.x, position.y);
		position = App->map->MapToWorld(placing_tile.x, placing_tile.y);

		rect.x = position.x;
		rect.y = position.y;

		if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
		{
			placed = true;
			tile = placing_tile;
		}

		if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
			CleanUp();
	}
	else
	{
		if (!selected)
		{
			if (team == 0)
				color.Blue();
			else if (team == 1)
				color.Red();
		}
		else
		{
			if (team == 0)
				color.SetColor(0u, 255u, 255u);
			else if (team == 1)
				color.SetColor(255u, 255u, 0u);

			if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
				placed = false;
			
			ShowHPbar(10, 5);
		}
	}

	App->render->AddBlitEvent(1, nullptr, 0, 0, rect, false, false, 0.0f, color.r, color.g, color.b, color.a);

	if (health == 0)
		CleanUp();
}

void BoatHouse::CleanUp()
{
	to_delete = true;
}
