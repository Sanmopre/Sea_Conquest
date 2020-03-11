#ifndef __j1Entities_H__
#define __j1Entities_H__

#include "p2Point.h"
#include "p2Log.h"
#include "j1App.h"
#include "SDL/include/SDL.h"
#include <vector>

#include "j1Render.h"
#include "Color.h"
#include "animation.h"
#include "j1Timer.h"

struct SDL_Texture;

enum class Orientation
{
	NORTH,
	NORTH_EAST,
	EAST,
	SOUTH_EAST,
	SOUTH,
	SOUTH_WEST,
	WEST,
	NORTH_WEST,
	NONE
};

enum class Entity_Type
{
	BOAT,
	BOATHOUSE,
	NONE
};

struct EntityRequest
{
	float x, y;
	Entity_Type type;
	int level;
	int team;
};

struct storage
{
	int wood;
	int cotton;
	int stone;
	int metal;

	int maxweight;
};
///////////////////////////////////////////////CLASSES//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Entity
{
public:

	Entity() { selected = false; to_delete = false; }

	virtual void Update(float dt) = 0;
	virtual void CleanUp() = 0;

	bool to_delete;

	void Damage(int damage, Entity* target)
	{
		if (target != nullptr)
		{
			target->health -= damage;
			if (target->health < 0)
				target->health = 0;
		}
	}

	int team;

	int health;
	int max_health;
	fPoint position;
	Entity_Type type;
	bool selected;
	int level;
	storage load;

	SDL_Rect rect; // /*probably will be*/ It is the current_animation
	SDL_Texture* texture;

protected:

	void  ShowHPbar(int extra_width, int height)
	{
		if (!showing_hpbar)
		{
			showing_hpbar = true;

			SDL_Rect health_rect = { position.x - extra_width, position.y - 20, /*animation.GetCurrentFrame().w*/ 32 + extra_width * 2, height };
			Color health_color(96u, 96u, 96u);

			App->render->AddBlitEvent(2, nullptr, 0, 0, health_rect, false, false, health_color.r, health_color.g, health_color.b, health_color.a);

			float hrw = health_rect.w;
			hrw /= max_health;
			hrw *= health;
			health_rect.w = hrw;
			health_color.SetColor(0u, 204u, 0u);

			App->render->AddBlitEvent(2, nullptr, 0, 0, health_rect, false, false, health_color.r, health_color.g, health_color.b, health_color.a);
		}
	}

	bool showing_hpbar;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Unit : public Entity
{
public:

	Unit() { orientation = Orientation::NORTH; }

	float speed;
	Orientation orientation;
	fPoint destination;

protected:

	Animation north;
	Animation north_east;
	Animation east;
	Animation south_east;
	Animation south;
	Animation south_west;
	Animation west;
	Animation north_west;

	fPoint past_frame_dest;
	//void Path_to(fPoint); Add when pathfinding is done
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Structure : public Entity
{
public:

	Structure() { placed = false; }

	iPoint tile;
	bool placed;

	void NotPlacedBehaviour();
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Resource : public Entity
{
public:

	int amount;

};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Boat : public Unit
{
public:
	Boat(float x = 0, float y = 0, int level = 1, int team = 0);
	~Boat();

	void Update(float);
	void CleanUp();

	int range;
	Entity* target;

private:

	void Move(float dt);
	void SetDestination();
	void Attack();
	void FindTarget();
	void SelectAnimation();

	timed_var firerate;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class BoatHouse : public Structure
{
public:
	BoatHouse(int team = 0, iPoint tile = {0, 0});
	~BoatHouse();

	void Update(float);
	void CleanUp();

	void BuildUnit(Entity_Type type, int level);

	std::vector<EntityRequest> unitqueue;
	timed_var building_time;
	Color color;
};
#endif // __j1Entities_H__