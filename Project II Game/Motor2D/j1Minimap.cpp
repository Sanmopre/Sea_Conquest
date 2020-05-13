#include "j1Minimap.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Textures.h"
#include "j1Input.h"

#include "j1SceneManager.h"
#include "j1Entities.h"
#include "j1EntityManager.h"
#include "j1Map.h"
#include "j1Fog.h"

#include "p2Log.h"
#include <list>

j1Minimap::j1Minimap()
{
	name = "minimap";
}

j1Minimap::~j1Minimap() {}

bool j1Minimap::Awake(pugi::xml_node & config)
{
	position.x= config.attribute("position.x").as_int(1013);
	position.y= config.attribute("position.y").as_int(560);

	return true;
}

bool j1Minimap::Start()
{
	minimap_tex = App->tex->GetTexture("minimap", 0, 0);
	noise_tex = App->tex->GetTexture("minimap-noise", 0, 0);

	fog_chunk_tex =	App->tex->GetTexture("fog-chunk", 0, 0);
	fog_sub_chunk_tex =	App->tex->GetTexture("fog-sub-chunk", 0, 0);
	fog_sub_sub_chunk_tex = App->tex->GetTexture("fog-sub-sub-chunk", 0, 0);
	fog_tile_tex = App->tex->GetTexture("fog-tile", 0, 0);

	map_anim = App->anim->GetAnimation("minimap");
	fog_chunk_anim = App->anim->GetAnimation("fog-chunk");
	fog_sub_chunk_anim = App->anim->GetAnimation("fog-sub-chunk");
	fog_sub_sub_chunk_anim = App->anim->GetAnimation("fog-sub-sub-chunk");
	fog_tile_anim = App->anim->GetAnimation("fog-tile");

	noise_tex = App->tex->GetTexture("minimap-noise", 0, 0);

	minimap_camera = App->tex->Load("textures/minimap_camera.png");

	map_rect = map_anim.GetCurrentFrame();
	tile_rect = fog_tile_anim.GetCurrentFrame();

	enemy = App->tex->Load("textures/enemy_unit.png");
	ally = App->tex->Load("textures/friend_unit.png");

	cotton = App->tex->Load("textures/cotton_mini.png");
	metal = App->tex->Load("textures/metal_mini.png");
	wood = App->tex->Load("textures/wood_mini.png");
	return true;
}

fPoint j1Minimap::MapToMinimap(int x, int y)
{
	fPoint ret;

	ret.x = (x - y) * (0.6) + map_rect.w / 2.0f;
	ret.y = (x + y) * (0.4);

	ret.x = map_position.x + ret.x;
	ret.y = map_position.y + ret.y;

	return ret;
}

void j1Minimap::RenderMinimapTile(int x, int y, SDL_Rect r)
{
	fPoint pos = MapToMinimap(x, y);
	//App->render->AddBlitEvent(5, fog_tile_tex, pos.x, pos.y, r);
	int grey = 100;
	if (App->input->GetKey(SDL_SCANCODE_0) != KEY_REPEAT)
		App->render->AddBlitEvent(8, nullptr, 0, 0, { (int)pos.x - 3,(int)pos.y,6,4 }, false, false, grey, grey, grey);
}

void j1Minimap::RenderMinimapFog(Chunk* chunk)
{
	for (vector<Chunk*>::iterator c = chunk->sub_chunks.begin(); c != chunk->sub_chunks.end(); c++)
	{
		Chunk* chunk = *c;
		if (chunk->score == 0)
		{
			fPoint tile = App->fog->GetChunkRenderTile(chunk);
			fPoint pos = MapToMinimap(tile.x, tile.y);

			SDL_Texture* texture = nullptr;
			SDL_Rect r = {};
			switch ((*c)->size)
			{
			case 40:
				texture = fog_chunk_tex;
				r = fog_chunk_anim.GetCurrentFrame();
				break;
			case 20:
				texture = fog_sub_chunk_tex;
				r = fog_sub_chunk_anim.GetCurrentFrame();
				break;
			case 10:
				texture = fog_sub_sub_chunk_tex;
				r = fog_sub_sub_chunk_anim.GetCurrentFrame();
				break;
			case 5:
				texture = fog_tile_tex;
				r = fog_tile_anim.GetCurrentFrame();
				break;
			}
			App->render->AddBlitEvent(5, texture, pos.x - r.w / 2, pos.y, r);
		}
		else if (!chunk->complete)
		{
			if(chunk->sub_chunks.size() != 0)
				RenderMinimapFog(*c);
			else
			{
				fPoint start = App->fog->GetChunkRenderTile(chunk);
				for (int y = start.y + 1; y < start.y + 12; y += 5)
					for (int x = start.x + 1; x < start.x + 12; x += 5)
						if (App->fog->map[x][y].state == FogState::FOGGED)
							RenderMinimapTile(x, y, tile_rect);
			}
		}
	}
}

bool j1Minimap::Update(float dt)
{	
	if (App->scenemanager->In_Logo_Scene != true && App->scenemanager->In_Main_Menu != true) 
	{
			MinimapToWorldCamera();
			map_position.x = position.x - App->render->camera.x - 70;
			map_position.y = position.y - App->render->camera.y - 190;

			App->render->AddBlitEvent(5, minimap_tex, map_position.x, map_position.y, map_rect, false, true, 0u, 0u, 0u, 255, true);

			RenderMinimapFog(App->fog->chunk_map);

			//App->render->AddBlitEvent(5, noise_tex, position.x - App->render->camera.x + 30, position.y - App->render->camera.y - 40, rect, false, true, 0u, 0u, 0u, 255, true);
			DrawCamera();
	}
	
	return true;
}

bool j1Minimap::PostUpdate()
{	
	return true;
}

bool j1Minimap::CleanUp()
{
	return true;
}

void j1Minimap::DrawCamera()
{
	App->render->AddBlitEvent(7, minimap_camera, 128 + position.x - App->render->camera.x - (App->render->camera.x / 50), position.y - App->render->camera.y - (App->render->camera.y / 50), cameraminimap, false);
}

void j1Minimap::MinimapToWorldCamera()
{
	//float x, y = 0;
	//App->input->GetMousePosFloat(x, y);
	//if (x > position.x&& y > position.y) {
	//	if (App->input->GetMouseButtonDown(1) == KEY_REPEAT) {
	//		clicking_map = true;
	//		App->render->camera.x = - ((x - position.x) * 50 - 6400) + 640;
	//		App->render->camera.y = - ((y - position.y) * 50 ) + 360;
	//	}
	//}
	//else {
	//	clicking_map = false;
	//}
}

void j1Minimap::Draw_entities()
{

	for (std::vector<j1Entity*>::iterator i = App->entitymanager->entities.begin(); i != App->entitymanager->entities.end(); i++)
	{
		if ((*i)->team == 1)
		{
			
			App->render->AddBlitEvent(6, ally,128 +  position.x + (*i)->position.x/50 - App->render->camera.x, position.y + (*i)->position.y / 50 - App->render->camera.y ,unit, false);

		}
		else if ((*i)->team == 2) {

			App->render->AddBlitEvent(6, enemy, 128 + position.x + (*i)->position.x / 50 - App->render->camera.x, position.y + (*i)->position.y / 50 - App->render->camera.y , unit, false);

		}
		else if ((*i)->type == EntityType::ALL_COTTON) {

			App->render->AddBlitEvent(6, cotton, 128 + position.x + (*i)->position.x / 50 - App->render->camera.x, position.y + (*i)->position.y / 50 - App->render->camera.y, unit, false);

		}
		else if ((*i)->type == EntityType::ALL_WOOD) {

			App->render->AddBlitEvent(6, wood, 128 + position.x + (*i)->position.x / 50 - App->render->camera.x, position.y + (*i)->position.y / 50 - App->render->camera.y, unit, false);

		}
		else if ((*i)->type == EntityType::ALL_METAL) {

			App->render->AddBlitEvent(6, metal, 128 + position.x + (*i)->position.x / 50 - App->render->camera.x, position.y + (*i)->position.y / 50 - App->render->camera.y, unit, false);

		}
			
	}
}



