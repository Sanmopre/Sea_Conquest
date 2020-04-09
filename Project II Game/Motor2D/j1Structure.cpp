#include "j1Entities.h"

#include "j1Input.h"
#include "j1Map.h"

j1Structure::j1Structure()
{ 
	main_type = EntityType::STRUCTURE;
	placed = false; 
}

void j1Structure::NotPlacedBehaviour()
{
	if (!placed)
	{
		position = App->input->GetMouseWorldPosition();

		iPoint placing_tile = App->map->WorldToMap(position.x, position.y);
		position = App->map->MapToWorld<fPoint>(placing_tile.x, placing_tile.y);

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
}