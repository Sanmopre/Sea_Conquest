#ifndef __j1Entities_H__
#define __j1Entities_H__

#include "p2Point.h"
#include "p2Log.h"
#include "j1App.h"
#include "SDL/include/SDL.h"
#include <vector>

#include "j1Render.h"
#include "Color.h"
#include "j1AnimationManager.h"
#include "j1Timer.h"
#include "j1Pathfinding.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Fog.h"

#define WOOD_MASS 4
#define COTTON_MASS 1
#define METAL_MASS 10

struct ParticleSystem;

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
	BALLOON,
	HARVESTER,
	CARRIER,
	BOATHOUSE,
	STORAGE,
	TOWNHALL,
	ALL_COTTON,
	ALL_WOOD,
	ALL_METAL,
	UNIT,
	STRUCTURE,
	RESOURCE,
	NONE
};

enum BuildState
{
	TO_BUILD,
	BUILDING,
	ON_HOLD,
	NOT_BUILDING,
	NOTHING
};

struct TextureInfo
{
	int level;
	EntityType type;
	SDL_Texture* texture;
	int team;
};

struct EntityRequest
{
	EntityRequest(float x, float y, EntityType type, int level, int team)
	{
		this->x = x;
		this->y = y;
		this->type = type;
		this->level = level;
		this->team = team;
	}
	float x, y;
	EntityType type;
	int level;
	int team;
};

enum Material
{
	WOOD,
	COTTON,
	METAL
};

struct Load
{
	int Total()
	{
		return wood + cotton + metal;
	}

	int Weight()
	{
		return WOOD_MASS*wood + COTTON_MASS*cotton + METAL_MASS*metal;
	}
	int WoodWeight()
	{
		return WOOD_MASS * wood;
	}
	int CottonWeight()
	{
		return COTTON_MASS * cotton;
	}
	int MetalWeight()
	{
		return METAL_MASS * metal;
	}

	void Transfer(Material material, int* sender_amount, int transfer_amount)
	{
		if (*sender_amount > 0)
		{
			int* mat = nullptr;
			int mass;
			switch (material)
			{
			case WOOD:
				mat = &wood;
				mass = WOOD_MASS;
				break;
			case COTTON:
				mat = &cotton;
				mass = COTTON_MASS;
				break;
			case METAL:
				mat = &metal;
				mass = METAL_MASS;
				break;
			}

			if (*sender_amount >= transfer_amount)
			{
				*mat += transfer_amount;
				*sender_amount -= transfer_amount;
			}
			else
			{
				int t = (transfer_amount - *sender_amount);
				if (t < 0)
					t = 0;
				*mat += t;
				*sender_amount = 0;
			}
			if (Weight() > maxweight)
			{
				int rest = (Weight() - maxweight) / mass;
				*sender_amount += rest;
				*mat -= rest;
			}
		}
	}

	int wood;
	int cotton;
	int metal;

	int maxweight;
};
///////////////////////////////////////////////CLASSES//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class j1Entity
{
public:

	j1Entity();
	virtual ~j1Entity();

	virtual void Primitive_Update(float dt) = 0;
	virtual void Update(float dt) = 0;
	virtual void CleanUp() = 0;

	bool to_delete;
	bool to_remove;
	vector<j1Entity*>::iterator spot;

	int team;

	float health;
	float max_health;
	fPoint position;
	EntityType type;
	EntityType main_type;
	NodeType terrain;
	bool selected;
	int level;
	int fog_range;

	Load load;
	j1Entity* trading_entity = nullptr;
	int trading_entity_offset = 0;
	int trading_offset_modifier;
	int trading_total;
	vector<j1Entity*>* GetStorages()
	{
		return &storages;
	}

	SDL_Rect selectable_area;

	SDL_Rect rect;
	SDL_Texture* texture;
	SDL_Texture* shadow;

	int GetRenderPositionX();
	int GetRenderPositionY();

	//UNITS
	virtual void GoTo(fPoint destination, NodeType terrain) {}
	virtual fPoint GetDestination() { return position; }
	virtual void SetDestination(fPoint destination) {}
	virtual void SetPosition(fPoint position) {}
	virtual void ResetPath() {}
	virtual void UpdateMap(Node* node) {}
		//HARVESTER
		virtual void SetAutomatic() {}
		virtual void BuildUnit(EntityType type, int level) {}
	//STRUCTURES
	virtual void BuildStructure(EntityType type) {}
	virtual void ToPlace(bool to_place) {}
	virtual void SetBuiltState(BuildState state) {}
	virtual BuildState GetBuiltState() { return NOTHING; }

protected:

	void  ShowHPbar(int extra_width, int height, int distance = 0);
	void Trading();
	j1Entity* FindTarget(float x, float y, int range, EntityType type, EntityType main_type, int team);
	
	bool showing_hpbar;
	int trading_range;

	std::vector<j1Entity*> tradeable_list;

	SDL_Texture* LoadTexture(j1Entity* entity, std::vector<TextureInfo>& textureBuffer);

	vector<j1Entity*> storages;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class j1Unit : public j1Entity
{
public:

	j1Unit();
	virtual ~j1Unit();
	void Primitive_Update(float dt);
	float speed;
	int range;
	Orientation orientation;
	fPoint destination;
	vector<fPoint> path;

	j1Entity* target;

	//PARTICLES
	bool Smoke;
	bool Fire;

	ParticleSystem* SmokeSystem;
	ParticleSystem* FireSystem;

	void UpdateMap(Node* node);

protected:

	Animation north;
	Animation north_east;
	Animation east;
	Animation south_east;
	Animation south;
	Animation south_west;
	Animation west;
	Animation north_west;

	vector<Node*>* map = nullptr;

	void GoTo(fPoint destination, NodeType terrain);
	void Move(float dt);
	void NextStep();
	void SetPosition(fPoint position);
	void SelectAnimation();
	void GetBasicAnimations();
	fPoint GetDestination() 
	{ 
		if(path.size() != 0)
			return *path.end(); 
		return destination;
	}
	void SetDestination(fPoint destination) { this->destination = destination; }
	void ResetPath()
	{
		if (path.size() != 0)
			path.erase(path.begin(), path.end());
		destination = { 0,0 };
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class j1Structure : public j1Entity
{
public:

	j1Structure();
	virtual ~j1Structure();
	void Primitive_Update(float dt);

	iPoint tile;
	bool placed;
	BuildState built_state;

	void NotPlacedBehaviour();
	void BuildUnit(EntityType type, int level);

	void ToPlace(bool to_place) { placed = to_place; }
	void SetBuiltState(BuildState state) { built_state = state; }
	BuildState GetBuiltState() { return built_state; }

	Animation* current_animation;

protected:

	Animation basic;
	Animation under_construction;

	SDL_Texture* texture;
	SDL_Texture* tex_construction;

	void BuildProcces(float dt);

	std::vector<EntityRequest> unitqueue;
	timed_var building_time;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class j1Resource : public j1Entity
{
public:

	j1Resource(float x, float y, int level = 1, EntityType type = EntityType::ALL_WOOD);
	~j1Resource() {};
	void Primitive_Update(float dt) {}

	Color color;

	void Update(float dt);
	void CleanUp();
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class j1Boat : public j1Unit
{
public:
	j1Boat(float x = 0, float y = 0, int level = 1, int team = 1);
	~j1Boat();

	void Update(float);
	void CleanUp();

private:

	void Damage(int damage, j1Entity* target);

	timed_var firerate;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class j1Balloon : public j1Unit
{
public:
	j1Balloon(float x = 0, float y = 0, int level = 1, int team = 1);
	~j1Balloon();

	void Update(float);
	void CleanUp();

private:

	void Damage(int damage, j1Entity* target);

	timed_var firerate;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class j1Harvester : public j1Unit
{
public:
	j1Harvester(float x, float y, int level = 1, int team = 1);
	~j1Harvester();

	void Update(float);
	void CleanUp();

	void SetAutomatic();

	void BuildStructure(EntityType type);
	BuildState GetBuiltState() { return state; }

private:

	void Harvest(int power, j1Entity* target);
	j1Entity* SearchResources(float x, float y);
	void BuildUpdate(float dt);

	j1Entity* building;
	BuildState state;

	bool automatic;
	bool automating;
	j1Entity* storage;
	fPoint harvest_destination;
	fPoint deposit_destination;

	timed_var harvestrate;
	timed_var transferrate;
	int power;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class j1Carrier : public j1Unit
{
public:

	j1Carrier(float x = 0, float y = 0, int level = 1, int team = 1);
	~j1Carrier();

	void Update(float);
	void CleanUp();

	void Store();
	void Deploy();

	int stored_units;
	int capacity;

private:

	vector<j1Entity*> units;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class j1BoatHouse : public j1Structure
{
public:
	j1BoatHouse(float x, float y, int team = 1);
	~j1BoatHouse();

	void Update(float);
	void CleanUp();
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class j1Storage : public j1Structure
{
public:
	j1Storage(float x, float y, int team = 1);
	~j1Storage();

	void Update(float);
	void CleanUp();

	int range;

private:

	void SearchStructures();
	Animation low;
	Animation half;
	Animation full;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
class j1TownHall : public j1Structure
{
public:
	j1TownHall(float x, float y, int team = 1);
	~j1TownHall();

	void Update(float);
	void CleanUp();
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // __j1Entities_H__