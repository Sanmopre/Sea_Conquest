#include "j1Entities.h"

#include "j1Render.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "j1ParticleManager.h"
#include "j1InGameUI.h"

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
	
	for (std::vector<Animation>::iterator i = App->entitymanager->allAnimations.begin(); i != App->entitymanager->allAnimations.end(); i++)
	{
		if (strcmp("entity_eight_north", i->name) == 0)
			this->north = i->GetAnimation();
		if (strcmp("entity_eight_northeast", i->name) == 0)
			this->north_east = i->GetAnimation();
		if (strcmp("entity_eight_east", i->name) == 0)
			this->east = i->GetAnimation();
		if (strcmp("entity_eight_southeast", i->name) == 0)
			this->south_east = i->GetAnimation();
		if (strcmp("entity_eight_south", i->name) == 0)
			this->south = i->GetAnimation();
		if (strcmp("entity_eight_southwest", i->name) == 0)
			this->south_west = i->GetAnimation();
		if (strcmp("entity_eight_west", i->name) == 0)
			this->west = i->GetAnimation();
		if (strcmp("entity_eight_northwest", i->name) == 0)
			this->north_west = i->GetAnimation();
	}

	for (std::vector<TextureInfo>::iterator e = App->entitymanager->allTextures.begin(); e != App->entitymanager->allTextures.end(); e++)
		if (this->type == e->type && this->level == e->level)
		{
			this->texture = e->texture; 
			break;
		}

	rect = north.GetCurrentFrame();
}

j1Boat::~j1Boat()
{

}

void j1Boat::Update(float dt)
{
	if (dt != 0.0f)
	{
		showing_hpbar = false;

		SelectAnimation();
		
		if (selected)
		{
			if (team == 1)
			{
				if (App->entitymanager->selected_n == 1 && App->input->GetMouseButtonDown(3) == KEY_DOWN)
					SetDestination();

				if(this == App->InGameUI->selected)
					Trading();
			}

			ShowHPbar(10, 5);
		}

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

		target = FindTarget(position.x, position.y, range, EntityType::NONE, -1);
	}

	App->render->AddBlitEvent(1, texture, GetRenderPositionX(), GetRenderPositionY(), rect);

	if (health == 0)
		CleanUp();
}

void j1Boat::CleanUp()
{
	path.erase(path.begin(), path.end());
	path.shrink_to_fit();
	to_delete = true;
}

void j1Boat::Damage(int damage, j1Entity* target)
{
	if (target->health != 0.0f)
	{
		target->health -= damage;

		if (target->health < 0)
			target->health = 0;

		//App->pmanager->createSystem(PARTICLE_TYPES::FIRE, target->position, 0.001f);

		if (target->health == 0)
			App->pmanager->createSystem(PARTICLE_TYPES::EXPLOSION, target->position, 1);

	}
}