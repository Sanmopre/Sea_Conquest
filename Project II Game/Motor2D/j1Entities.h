#ifndef __j1Entities_H__
#define __j1Entities_H__

#include "p2Point.h"
#include "p2Log.h"
#include "j1App.h"
#include "SDL/include/SDL.h"

#include "Color.h"

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
	NORTH_WEST
};

enum class Entity_Type
{
	BOAT,
	BOATHOUSE,
	NONE
};
///////////////////////////////////////////////CLASSES//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Entity
{
public:

	Entity() { selected = false; to_delete = false; }

	virtual void Update(float dt) {};
	virtual void CleanUp() {};

	bool to_delete;

	void Damage(int damage, Entity* target)
	{
		target->health -= damage;
		if (target->health < 0)
			target->health = 0;
	}

	int health;
	int max_health;
	iPoint position;
	Entity_Type type;
	bool selected;
	int level;

	SDL_Rect rect; //probably will be the future current_animation
	SDL_Texture* texture;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Unit : public Entity
{
public:

	int speed;
	int range;
	Orientation looking;
	iPoint destination;

protected:

	iPoint past_frame_dest;
	//void Path_to(fPoint); Add when pathfinding is done
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Structure : public Entity
{
public:

	iPoint tile;

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
	Boat();
	Boat(float x, float y, int level);
	~Boat();

	void Update(float);
	void CleanUp();

	Color color;
	Entity* target;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class BoatHouse : public Structure
{
public:
	BoatHouse();
	~BoatHouse();

	void Update(float);
	void CleanUp();

	bool placed;

	Color color;
};
#endif // __j1Entities_H__