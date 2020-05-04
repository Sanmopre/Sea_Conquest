#include "j1Entities.h"

#include "j1Render.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "j1ParticleManager.h"
#include "j1Player.h"

#include <vector>

j1Boat::j1Boat(float x, float y, int level, int team)
{
	type = EntityType::BOAT;
	terrain = NodeType::WATER;
	position.x = x;
	position.y = y;
	destination = position;
	this->level = level;
	trading_range = 50;
	this->team = team;
	speed = 50;
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

	GetBasicAnimations();

	App->audio->PlaySpatialFx(App->audio->boat_spawn,
		App->audio->GetAngle(App->render->getCameraPosition(), { (int)position.x, (int)position.y }),
		App->audio->GetDistance(App->render->getCameraPosition(), { (int)position.x, (int)position.y }));
}

j1Boat::~j1Boat()
{

}

void j1Boat::Update(float dt)
{
	if (dt != 0.0f)
	{	
		if (selected)
		{
			if (team == 1)
			{
				if (App->entitymanager->selected_n == 1 && App->input->GetMouseButtonDown(3) == KEY_DOWN && !App->player->disable_click)
					SetDestination(terrain);
			}
		}

		target = FindTarget(position.x, position.y, range, EntityType::NONE, -1);

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
					Damage(10, target);
					firerate.counter = 0;
				}
			}
		}

		//PARTICLES
		if (health < 0)
		{
			health = 0;
			//App->audio->PlayFx(App->audio->boat_destroy);
			App->audio->PlaySpatialFx(App->audio->boat_destroy,
				App->audio->GetAngle(App->render->getCameraPosition(), { (int)position.x, (int)position.y }),
				App->audio->GetDistance(App->render->getCameraPosition(), { (int)position.x, (int)position.y }));
		}

		if (health != 0.0f)
		if (health != 0)
		{
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
		}	
	}

	App->render->AddBlitEvent(1, texture, GetRenderPositionX(), GetRenderPositionY(), rect);

	if (health == 0)
	{
		CleanUp();
		App->pmanager->createSystem(PARTICLE_TYPES::EXPLOSION, position, 0.9);
	}
}

void j1Boat::CleanUp()
{
	path.erase(path.begin(), path.end());
	path.shrink_to_fit();
	if(SmokeSystem != nullptr)
		SmokeSystem->toDelete = true;
	if (FireSystem != nullptr)
		FireSystem->toDelete = true;
	to_delete = true;
}

void j1Boat::Damage(int damage, j1Entity* target)
{
	if (target->health != 0)
	{
		target->health -= damage;
		App->audio->PlaySpatialFx(App->audio->boat_attack,
			App->audio->GetAngle(App->render->getCameraPosition(), { (int)position.x, (int)position.y }),
			App->audio->GetDistance(App->render->getCameraPosition(), { (int)position.x, (int)position.y }));
		if (target->health < 0)
			target->health = 0;
	}
}