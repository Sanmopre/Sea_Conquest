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
			(*itr)->state = FogState::PARTIAL;
		vector<FogTile*> v;
		last_frame.swap(v);
		
		for (vector<j1Entity*>::iterator e = App->entitymanager->ally_entities.begin(); e != App->entitymanager->ally_entities.end(); e++)
		{
			j1Entity* entity = *e;
			iPoint tile = App->map->WorldToMap(entity->position.x, entity->position.y - 16);
			int range = entity->fog_range;
		
			for (int y = tile.y - range + 1; y < tile.y + range + 2; y++)
				for (int x = tile.x - range + 1; x < tile.x + range + 2; x++)
				{
					FogTile* fog = &map[x][y];
		
					fog->state = FogState::VISIBLE;
		
					last_frame.push_back(fog);
				}
		}
	}

	return true;
}

bool j1Fog::CleanUp()
{
	vector<FogTile*> v;
	last_frame.swap(v);

	for (int i = 0; i < width; ++i)
		delete[] map[i];
	delete[] map;

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