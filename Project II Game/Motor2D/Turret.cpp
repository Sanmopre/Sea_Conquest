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

Turret::Turret(float x, float y, int team)
{
	type = EntityType::TURRET;
	terrain = NodeType::ALL;
	fog_range = 7;

	iPoint tile = App->map->WorldToMap(x, y);
	position = App->map->MapToWorld<fPoint>(tile.x, tile.y);

	level = 1;
	trading_range = 0;
	this->team = team;

	damage = 20;
	range = 150;
	firerate = { 0.25 };

	max_health = 500;
	if (built_state != BUILDING)
		health = max_health;

	load = { 0, 0, 0, 0 };
	target = nullptr;

	texture = App->tex->GetTexture("turret", level, team);
	tex_construction = App->tex->GetTexture("cons_small", 0, 0);

	basic = App->anim->GetAnimation("turret");
	under_construction = App->anim->GetAnimation("cons_small");

	selectable_area = rect;

	(*App->pathfinding->WorldToNode(tile.x, tile.y))->built = true;

	current_animation = &basic;
}

Turret::~Turret()
{

}

void Turret::Update(float dt)
{
	if (dt != 0.0f && built_state != BUILDING && built_state != ON_HOLD && built_state != TO_BUILD)
	{
		int enemy = 0;
		if (team == 1)
			enemy = 2;
		else if (team == 2)
			enemy = 1;

		target = FindTarget(position.x, position.y, range, EntityType::NONE, EntityType::NONE, enemy);

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

	if (selected)
		App->render->AddBlitEvent(0, nullptr, 1, 0, { (int)position.x,(int)position.y, range, 0 }, false, false, 255, 0, 0);

	if (App->fog->GetVisibility(position) == FogState::VISIBLE || App->ignore_fog)
	{
		App->render->AddBlitEvent(1, current_tex, GetRenderPositionX(), GetRenderPositionY(), rect);
		App->minimap->Draw_entities(this);
	}
}

void Turret::CleanUp()
{
	to_delete = true;

	//CHECK QUEST 
	if (App->quest->current_quest == QUEST::DESTROY_ENEMY_STRUCTURE) 
		App->quest->main_quest.current++;
}

void Turret::Damage(int damage, j1Entity* target)
{
	if (target->health != 0)
	{
		target->health -= damage;

		if (target->health < 0)
			target->health = 0;

		this->target = nullptr;
	}
}

void Turret::GetStats(int& attack, int& health, int& maxhealth, int& speed, int& maxresources)
{
	attack = damage;
	health = this->health;
	maxhealth = max_health;
	speed = 0;
	maxresources = load.maxweight;
}