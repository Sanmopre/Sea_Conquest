#ifndef _FOG_H_
#define _FOG_H_

#include "j1Module.h"
#include "p2Point.h"
#include "j1AnimationManager.h"

#include <vector>

using namespace std;

struct SDL_Texture;

enum class FogState
{
	FOGGED,
	PARTIAL,
	VISIBLE
};

struct FogTile
{
	FogTile()
	{
		state = FogState::FOGGED;
	}
	FogState state;
};

class j1Fog : public j1Module
{
public:

	j1Fog();
	~j1Fog();

	bool Update(float dt);
	bool CleanUp();

	void LoadMap(int width, int height);

	void RenderFogTile(int x, int y, int alpha);
	FogState GetVisibility(int x, int y);

	FogTile** map;

private:

	SDL_Texture* fog_texture;
	Animation fog_anim;

	int width;
	vector<FogTile*> last_frame;
};

#endif //_FOG_H_