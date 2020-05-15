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
#include "j1Entities.h"

#include "p2Log.h"
#include <list>

j1Minimap::j1Minimap()
{
	name = "minimap";
}

j1Minimap::~j1Minimap() {}

bool j1Minimap::Awake(pugi::xml_node & config)
{
	position.x= 1023;
	position.y= 540;

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

	minimap_camera = App->tex->GetTexture("minimap-cam", 0, 0);

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

	ret.x = position.x + ret.x;
	ret.y = position.y + ret.y;

	return ret;
}
iPoint j1Minimap::MinimapToMap(float x, float y)
{
	iPoint ret;
	x -= map_position.x;
	y -= map_position.y;

	ret.x = int((x / 0.6 + y / 0.4) / 2);
	ret.y = int((y / 0.4 - (x / 0.6)) / 2);
	
	return ret;
}


void j1Minimap::RenderMinimapTile(int x, int y, SDL_Rect r)
{
	fPoint pos = MapToMinimap(x, y);

	int grey = 60;
	if (App->input->GetKey(SDL_SCANCODE_0) != KEY_REPEAT)
		App->render->AddBlitEvent(5, nullptr, 0, 0, { (int)pos.x - 3,(int)pos.y,6,4 }, false, true, grey, grey, grey, 255, true);
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
			App->render->AddBlitEvent(5, texture, pos.x - r.w / 2, pos.y, r, false, true, 0, 0, 0, 255, true);
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
			map_position.x = position.x - App->render->camera.x;
			map_position.y = position.y - App->render->camera.y;

			App->render->AddBlitEvent(5, minimap_tex, position.x, position.y, map_rect, false, true, 0, 0, 0, 255, true);

			if(!App->ignore_fog)
				RenderMinimapFog(App->fog->chunk_map);

			App->render->AddBlitEvent(5, noise_tex, position.x, position.y, map_rect, false, true, 0, 0, 0, 200, true);
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
	iPoint minimap_cam_pos = { -App->render->camera.x, -App->render->camera.y };
	minimap_cam_pos.x = (minimap_cam_pos.x * 120) / 6400;
	minimap_cam_pos.y = (minimap_cam_pos.y * 160) / 6400;
	App->render->AddBlitEvent(10, minimap_camera, position.x + minimap_cam_pos.x + map_rect.w / 2, position.y + minimap_cam_pos.y, cameraminimap, false, true, 0, 0, 0, 255, true);
}

void j1Minimap::MinimapToWorldCamera()
{
	iPoint cam_pos;
	App->input->GetMousePosition(cam_pos.x, cam_pos.y);
	clicking_map = false;
	if (cam_pos.x > position.x && cam_pos.y > position.y && !App->player->dragging) 
		if (App->input->GetMouseButtonDown(1) == KEY_REPEAT) 
		{
			clicking_map = true;
			cam_pos.x += -App->render->camera.x - map_rect.w / 2;
			cam_pos.y += -App->render->camera.y;
			
			cam_pos = MinimapToMap(cam_pos.x, cam_pos.y);

			if (cam_pos.x > 12 && cam_pos.x < App->map->mapdata->width && cam_pos.y > 12 && cam_pos.y < App->map->mapdata->height)
			{

				cam_pos = App->map->MapToWorld<iPoint>(cam_pos.x, cam_pos.y);
				cam_pos.x -= App->win->width / 2;
				cam_pos.y -= App->win->height / 2;

				App->render->camera.x = -cam_pos.x;
				App->render->camera.y = -cam_pos.y;
			}
		}
}

void j1Minimap::Draw_entities(j1Entity* entity)
{
	Color color;
	if (entity->team == 1)
		color.Green();
	else if (entity->team == 2) 
		color.Red();
	else if (entity->type == EntityType::ALL_COTTON) 
		color.SetColor(240u, 240u, 240u);
	else if (entity->type == EntityType::ALL_WOOD) 
		color.SetColor(120u, 72u, 0u);
	else if (entity->type == EntityType::ALL_METAL) 
		color.SetColor(107u, 120u, 119u);

	iPoint tile = App->map->WorldToMap(entity->position.x, entity->position.y);
	fPoint pos = MapToMinimap(tile.x, tile.y);

	App->render->AddBlitEvent(6, nullptr, 0, 0, {(int)pos.x,(int)pos.y, 2, 2}, false, true, color.r, color.g, color.b, 255, true);
}



