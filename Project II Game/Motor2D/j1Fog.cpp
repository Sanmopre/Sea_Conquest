#include "j1Fog.h"
#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1Textures.h"

#include "j1Render.h"

j1Fog::j1Fog()
{
}

j1Fog::~j1Fog()
{
	CleanUp();
}

bool j1Fog::Update(float dt)
{
	if (map != nullptr)
	{
		for (vector<FogTile*>::iterator itr = last_frame.begin(); itr != last_frame.end(); itr++)
		{
			(*itr)->state = FogState::PARTIAL;
			if (!(*itr)->discovered)
			{
				(*itr)->discovered = true;
				AddScore((*itr)->chunk);
			}
		}

		vector<FogTile*> v;
		last_frame.swap(v);
		
		if(App->entitymanager->entities.size() != 0)
		for (vector<j1Entity*>::iterator e = App->entitymanager->ally_entities.begin(); e != App->entitymanager->ally_entities.end(); e++)
		{
			j1Entity* entity = *e;
			iPoint tile = App->map->WorldToMap(entity->position.x, entity->position.y - 16);
			int range = entity->fog_range;
		
			for (int y = tile.y - range + 1; y < tile.y + range + 2; y++)
				for (int x = tile.x - range + 1; x < tile.x + range + 2; x++)
				{
					if (x >= App->map->mapdata->width || y >= App->map->mapdata->height || x < 0 || y < 0)
					{
					}
					else
					{
						FogTile* fog = &map[x][y];
		
						fog->state = FogState::VISIBLE;
		
						last_frame.push_back(fog);
					}
				}
		}
	}

	return true;
}

bool j1Fog::CleanUp()
{
	vector<FogTile*> v;
	last_frame.swap(v);

	if (map != nullptr)
	{
		for (int i = 0; i < width; ++i)
			delete[] map[i];
		delete[] map;
	}
	map = nullptr;

	return true;
}

void j1Fog::LoadMap(int width, int height)
{
	CleanUp();

	fog_texture = App->tex->GetTexture("fog_tile", 0, 0);
	fog_anim = App->anim->GetAnimation("tile_highlight");

	map = new FogTile* [width];
	for (int i = 0; i < width; ++i)
		map[i] = new FogTile[height];

	chunk_map = new Chunk(0, 4, width / quality, nullptr);

	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			map[x][y].chunk = GetChunk(x, y);
	
	this->width = width;
}

void j1Fog::RenderFogTile(int x, int y, int alpha)
{
	fPoint position = App->map->MapToWorld<fPoint>(x, y);
	SDL_Rect r = fog_anim.GetCurrentFrame();
	App->render->AddBlitEvent(3, fog_texture, position.x - 32, position.y - 32, r, false, false, 0, 0, 0, alpha);
}

FogState j1Fog::GetVisibility(int x, int y)
{
	return map[x][y].state;
}

FogState j1Fog::GetVisibility(fPoint pos)
{
	iPoint tile = App->map->WorldToMap(pos.x, pos.y);

	return map[tile.x][tile.y].state;
}

int j1Fog::GetSubChunkId(int x, int y, fPoint size)
{
	size.x *= quality;
	size.y *= quality;

	if (x >= size.x)
		if (y >= size.y)
			return 3;
		else
			return 1;
	else
		if (y >= size.y)
			return 2;
		else
			return 0;
}

Chunk* j1Fog::GetChunk(int x, int y)
{
	Chunk* chunk = chunk_map;
	fPoint size = { 0,0 };

	while (chunk->sub_chunks.size() != 0)
	{
		size.x += chunk->size / 2;
		size.y += chunk->size / 2;

		int id = GetSubChunkId(x, y, size);

		if (id == 0 || id == 2)
			size.x -= chunk->size / 2;
		if (id == 0 || id == 1)
			size.y -= chunk->size / 2;

		chunk = *(chunk->sub_chunks.begin() + id);
	}

	return chunk;
}

fPoint j1Fog::GetChunkRenderTile(Chunk* chunk)
{
	fPoint ret = { 0,0 };

	while (chunk->parent != nullptr)
	{
		if (chunk->id == 0 || chunk->id == 1)
			ret.x += chunk->size * chunk->id;
		else
		{
			ret.x += chunk->size * (chunk->id - 2);
			ret.y += chunk->size;
		}

		chunk = chunk->parent;
	}

	ret.x *= quality;
	ret.y *= quality;

	return ret;
}

void j1Fog::AddScore(Chunk* chunk)
{
	chunk->score++;
	if(chunk->score == 625)
		chunk->complete = true;
	if (!chunk->discovered)
	{
		chunk->discovered = true;
	
		chunk->parent->score++;
	}

	while (chunk->parent->parent != nullptr)
	{
		chunk = chunk->parent;

		if (!chunk->discovered)
		{
			chunk->discovered = true;

			chunk->parent->score++;
		}

		if (!chunk->complete && chunk->IsComplete())
			chunk->complete = true;
	}
}

bool j1Fog::Load(pugi::xml_node& data)
{
	pugi::xml_node node = data.child("fog");

	return true;
}

bool j1Fog::Save(pugi::xml_node& data) const
{
	pugi::xml_node node = data.append_child("fog");



	return true;
}