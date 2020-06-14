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

struct Chunk
{
	Chunk() {}
	Chunk(int id, int sub_chunks, int size, Chunk* parent)
	{
		this->id = id;
		this->parent = parent;
		this->size = size;

		size /= 2;
		if (size >= 5)
		{
			for (int i = 0; i < 4; i++)
				this->sub_chunks.push_back(new Chunk(i, sub_chunks, size, this));
		}
	}

	~Chunk()
	{
		while (sub_chunks.size() != 0)
		{
			delete *sub_chunks.begin();
			sub_chunks.erase(sub_chunks.begin());
		}
		vector<Chunk*> v;
		sub_chunks.swap(v);
	}

	bool IsComplete()
	{
		int counter = 0;

		for (vector<Chunk*>::iterator itr = sub_chunks.begin(); itr != sub_chunks.end(); itr++)
			if (!(*itr)->complete)
				return false;

		return true;
	}

	Chunk* parent;
	vector<Chunk*> sub_chunks;

	int size;

	int id;
	int score = 0;

	bool discovered = false;
	bool complete = false;
};

struct FogTile
{
	FogState state = FogState::FOGGED;
	bool discovered = false;

	Chunk* chunk;
};

class j1Fog : public j1Module
{
public:

	j1Fog();
	~j1Fog();

	bool Load(pugi::xml_node& data);
	bool Save(pugi::xml_node& data) const;

	bool Update(float dt);
	bool CleanUp();

	void LoadMap(int width, int height);

	void RenderFogTile(int x, int y, int alpha);
	FogState GetVisibility(int x, int y);
	FogState GetVisibility(fPoint pos);

	FogTile** map;

	Chunk* chunk_map;

	fPoint GetChunkRenderTile(Chunk* chunk);

	const float quality = 2.5f;

private:

	int GetSubChunkId(int x, int y, fPoint size);
	Chunk* GetChunk(int x, int y);
	void AddScore(Chunk* chunk);

	SDL_Texture* fog_texture;
	Animation fog_anim;

	int width;
	vector<FogTile*> last_frame;
};

#endif //_FOG_H_