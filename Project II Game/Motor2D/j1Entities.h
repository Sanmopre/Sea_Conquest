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
#include "j1Pathfinding.h"

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

enum class EntityType
{
	BOAT,
	BOATHOUSE,
	HARVESTER,
	WOOD_RESOURCE,
	COTTON_RESOURCE,
	METAL_RESOURCE,
	EVEN2_RESOURCE,
	EVEN3_RESOURCE,
	NONE
};

struct TextureInfo
{
	int level;
	EntityType type;
	SDL_Texture* texture;
};

struct EntityRequest
{
	float x, y;
	EntityType type;
	int level;
	int team;
};

struct Storage
{
	int wood;
	int cotton;
	int metal;

	int maxweight;
};
///////////////////////////////////////////////CLASSES//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class j1Entity
{
public:

	j1Entity() { selected = false; to_delete = false; }
	~j1Entity();

	virtual void Update(float dt) = 0;
	virtual void CleanUp() = 0;

	bool to_delete;



	int team;

	int health;
	int max_health;
	fPoint position;
	EntityType type;
	bool selected;
	int level;

	Storage storage;
	j1Entity* trading_entity;
	int trading_entity_offset = 0;

	SDL_Rect rect;
	SDL_Texture* texture;

	int GetRenderPositionX();
	int GetRenderPositionY();

	virtual void BuildUnit(EntityType type, int level) {}

protected:

	void  ShowHPbar(int extra_width, int height);
	void Trading();
	j1Entity* FindTarget(int range, EntityType type);

	bool showing_hpbar;
	int trading_range;

	std::vector<j1Entity*> tradeable_list;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class j1Unit : public j1Entity
{
public:

	j1Unit() { orientation = Orientation::NORTH; }
	~j1Unit();

	float speed;
	Orientation orientation;
	fPoint destination;
	vector<fPoint> path;

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
	void GoTo(fPoint destination, NodeType terrain);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class j1Structure : public j1Entity
{
public:

	j1Structure() { placed = false; }

	iPoint tile;
	bool placed;

	void NotPlacedBehaviour();
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class j1Resource : public j1Entity
{
public:

	j1Resource(float x = 0, float y = 0, int level = 1, EntityType type = EntityType::EVEN3_RESOURCE);

	void Update(float dt);
	void CleanUp();
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class j1Boat : public j1Unit
{
public:
	j1Boat(float x = 0, float y = 0, int level = 1, int team = 0);
	~j1Boat();

	void Update(float);
	void CleanUp();

	int range;
	j1Entity* target;

private:

	void Move(float dt);
	void NextStep();
	void SetDestination();
	void SelectAnimation();
	void Damage(int damage, j1Entity* target);

	timed_var firerate;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class j1BoatHouse : public j1Structure
{
public:
	j1BoatHouse(int team = 0, iPoint tile = {0, 0});
	~j1BoatHouse();

	void Update(float);
	void CleanUp();

	void BuildUnit(EntityType type, int level);

	std::vector<EntityRequest> unitqueue;
	timed_var building_time;
	Color color;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class j1Island : public j1Resource
{
public:

};
#endif // __j1Entities_H__