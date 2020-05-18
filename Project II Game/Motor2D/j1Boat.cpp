#include "j1Entities.h"

#include "j1Render.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "j1ParticleManager.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1Minimap.h"
#include "j1QuestManager.h"

#include <vector>

j1Boat::j1Boat(float x, float y, int level, int team)
{
	type = EntityType::BOAT;
	terrain = NodeType::WATER;
	fog_range = 5;

	iPoint tile = App->map->WorldToMap(x, y);
	position = App->map->MapToWorld<fPoint>(tile.x, tile.y);
	destination = position;
	this->level = level;
	trading_range = 50;
	this->team = team;
	int extra = level * 1.5;
	speed = 50 + 10 * extra;
	damage = 10 + 5 * extra;
	range = 100;
	firerate = { 1 };
	max_health = 100;
	health = max_health;
	load = { 0, 0, 0, 200 };
	target = nullptr;

	//PARTICLES TEST
	Smoke = false;
	Fire = false;
	SmokeSystem = nullptr;
	FireSystem = nullptr;
	//

	texture = App->tex->GetTexture("boat", level, team);
	shadow = App->tex->GetTexture("boat-shadow", 0, 0);

	GetBasicAnimations();
	selectable_area = rect;

	App->audio->PlaySpatialFx(App->audio->boat_spawn,
		App->audio->GetAngle(App->render->getCameraPosition(), { (int)position.x, (int)position.y }),
		App->audio->GetDistance(App->render->getCameraPosition(), { (int)position.x, (int)position.y }));

	if (App->quest->current_quest == QUEST::BUILD_BOAT)
		App->quest->main_quest.current++;

	if (App->quest->current_quest == QUEST::BUILD_10_BOATS)
		App->quest->main_quest.current++;
}

j1Boat::~j1Boat()
{

}

void j1Boat::Update(float dt)
{
	if (dt != 0.0f)
	{
		int enemy = 0;
		if (team == 1)
			enemy = 2;
		else if (team == 2)
			enemy = 1;

		target = FindTarget(position.x, position.y, range, EntityType::NONE, EntityType::NONE, enemy);

		Chase(range + 50, enemy);

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
		//PARTICLES
		if (health < max_health / 2 && !Smoke)
		{
			SmokeSystem = App->pmanager->createSystem(PARTICLE_TYPES::SMOKE, position, 0);
			Smoke = true;
		}
		if (health < max_health / 5 && !Fire)
		{
			FireSystem = App->pmanager->createSystem(PARTICLE_TYPES::FIRE, position, 0);
			Fire = true;
		}

		if (Smoke)
			SmokeSystem->changePosition(position);
		if (Fire)
			FireSystem->changePosition(position);

		App->render->AddBlitEvent(1, shadow, GetRenderPositionX(), GetRenderPositionY(), rect, false, false, 0, 0, 0, 100);
		App->render->AddBlitEvent(1, texture, GetRenderPositionX(), GetRenderPositionY(), rect);
		App->minimap->Draw_entities(this);
	}
}

void j1Boat::CleanUp()
{
	if (App->entitymanager->townhall_level != 0)
	{
		App->pmanager->createSystem(PARTICLE_TYPES::EXPLOSION, position, 0.9);

		App->audio->PlaySpatialFx(App->audio->boat_destroy,
			App->audio->GetAngle(App->render->getCameraPosition(), { (int)position.x, (int)position.y }),
			App->audio->GetDistance(App->render->getCameraPosition(), { (int)position.x, (int)position.y }));
	}

	path.erase(path.begin(), path.end());
	path.shrink_to_fit();
	if(SmokeSystem != nullptr)
		SmokeSystem->toDelete = true;
	if (FireSystem != nullptr)
		FireSystem->toDelete = true;
	to_delete = true;

	//CHECK QUEST 
	if (App->quest->current_quest == QUEST::KILL_15_BOATS && team == 2) 
		App->quest->main_quest.current++;
}

void j1Boat::Damage(int damage, j1Entity* target)
{
	if (target->health != 0)
	{
		target->health -= damage;

		App->audio->PlaySpatialFx(App->audio->boat_attack,
			App->audio->GetAngle(App->render->getCameraPosition(), { (int)position.x, (int)position.y }),
			App->audio->GetDistance(App->render->getCameraPosition(), { (int)position.x, (int)position.y }));

		App->pmanager->TheCannonSystem->shootCannonBall(position, target->position);

		if (target->health < 0)
			target->health = 0;
	
		this->target = nullptr;
	}
}