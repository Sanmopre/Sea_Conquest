#include "j1Entities.h"

#include "j1Render.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "j1ParticleManager.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1Minimap.h"

#include <vector>

j1Balloon::j1Balloon(float x, float y, int level, int team)
{
	type = EntityType::BALLOON;
	terrain = NodeType::ALL;
	fog_range = 7;

	iPoint tile = App->map->WorldToMap(x, y);
	position = App->map->MapToWorld<fPoint>(tile.x, tile.y);
	destination = position;
	this->level = level;
	trading_range = 10;
	this->team = team;
	int extra = level * 1.5;
	speed = 80 + 15 * extra;
	damage = 50 + 10 * extra;
	range = 10;
	firerate = { 3 };
	max_health = 50;
	health = max_health;
	load = { 0, 0, 0, 50 };
	target = nullptr;

	texture = App->tex->GetTexture("balloon", level, team);
	shadow = App->tex->GetTexture("balloon-shadow", level, 0);

	GetBasicAnimations();
	selectable_area = rect;
}

j1Balloon::~j1Balloon()
{
	CleanUp();
}

void j1Balloon::Update(float dt)
{
	if (dt != 0.0f)
	{
		int enemy = 0;
		if (team == 1)
			enemy = 2;
		else if (team == 2)
			enemy = 1;

		target = FindTarget(position.x, position.y, range, EntityType::NONE, EntityType::NONE, enemy);

		Chase(range + 200, enemy);

		if (destination != position)
			Move(dt);
		else
		{
			NextStep();

			if (target != nullptr)
			{
				firerate.counter += dt;
				if (firerate.counter >= firerate.iterations)
				{
					Damage(damage, target);
					firerate.counter = 0;
				}
			}
		}
	}

	if (App->fog->GetVisibility(position) == FogState::VISIBLE || App->ignore_fog)
	{
		App->render->AddBlitEvent(1, shadow, GetRenderPositionX(), GetRenderPositionY() + 50, rect, false, false, 0, 0, 0, 100);
		App->render->AddBlitEvent(1, texture, GetRenderPositionX(), GetRenderPositionY(), rect);
		App->minimap->Draw_entities(this);
	}

	if (health <= 0)
	{
		CleanUp();
		App->pmanager->createSystem(PARTICLE_TYPES::EXPLOSION, position, 0.9);
	}
}

void j1Balloon::CleanUp()
{
	path.erase(path.begin(), path.end());
	path.shrink_to_fit();

	to_delete = true;
}

void j1Balloon::Damage(int damage, j1Entity* target)
{
	if (target->health != 0)
	{
		target->health -= damage;

		if (target->health < 0)
			target->health = 0;
	}
}