#include "j1Entities.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Input.h"

#include "j1EntityManager.h"
#include "j1Map.h"

BoatHouse::BoatHouse()
{
	type = Entity_Type::BOATHOUSE;
	App->input->GetMousePosition(position.x, position.y);
	position.x -= App->render->camera.x / App->win->GetScale();
	position.y -= App->render->camera.y / App->win->GetScale();
	level = 1;
	max_health = 500;
	health = max_health;
	placed = false;
}

BoatHouse::~BoatHouse()
{

}

void BoatHouse::Update(float dt)
{
	if (health == 0)
		to_delete = true;

	SDL_Rect rect = { position.x, position.y, 40, 40 };

	if (!placed)
	{
		color.SetColor(0u, 100u, 255u);

		App->input->GetMousePosition(position.x, position.y);
		position.x -= App->render->camera.x / App->win->GetScale();
		position.y -= App->render->camera.y / App->win->GetScale();

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
		{
			to_delete = true;
		}
	}
	else
	{
		if (!selected)
		{
			color.SetColor(0u, 100u, 255u);
		}
		else
		{
			if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
			{
				placed = false;
			}
			color.SetColor(0u, 255u, 255u);

			SDL_Rect health_rect = { rect.x - 10, rect.y - 20, rect.w + 20, 5 };
			Color health_color(96u, 96u, 96u);

			App->render->AddBlitEvent(2, nullptr, 0, 0, health_rect, false, 0.0f, health_color.r, health_color.g, health_color.b, health_color.a);

			float hrw = health_rect.w;
			hrw /= max_health;
			hrw *= health;
			health_rect.w = hrw;
			health_color.SetColor(0u, 204u, 0u);

			App->render->AddBlitEvent(2, nullptr, 0, 0, health_rect, false, 0.0f, health_color.r, health_color.g, health_color.b, health_color.a);
		}
	}

	App->render->AddBlitEvent(1, nullptr, 0, 0, rect, false, 0.0f, color.r, color.g, color.b, color.a);
}

void BoatHouse::CleanUp()
{

}