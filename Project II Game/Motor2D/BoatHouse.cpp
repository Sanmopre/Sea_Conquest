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
		
		ShowHPbar(10, 5);
	}
	else
		if (team == 0)
			color.Blue();
		else if (team == 1)
			color.Red();
	
	App->render->AddBlitEvent(1, nullptr, 0, 0, rect, false, false, color.r, color.g, color.b, color.a);

	if (health == 0)
		CleanUp();
}

void BoatHouse::CleanUp()
{
	to_delete = true;
}
