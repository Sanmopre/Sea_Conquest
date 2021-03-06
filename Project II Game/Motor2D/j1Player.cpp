#include "p2Defs.h"
#include "j1App.h"
#include "p2Log.h"
#include <math.h>
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1Window.h"
#include "j1GUI.h"
#include "j1Minimap.h"
#include "j1InGameUI.h"
#include "j1SceneManager.h"
#include "j1Map.h"
#include "j1Fog.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "j1EntityManager.h"

#define X_DISTANCE 30
#define Y_DISTANCE 30

using namespace std;

struct {
	bool operator()(fPoint a, fPoint b) const
	{
		if (a.y != b.y)
			return a.y < b.y;
		else
			return a.x < b.x;
	}
} LessY;

j1Player::j1Player() : j1Module()
{

	name.create("player");
}


j1Player::~j1Player()
{

}

bool j1Player::Awake(pugi::xml_node& config)
{
	bool ret = true;

	folder.create(config.child("folder").child_value());

	camera_speed = config.child("camera").attribute("speed").as_int(2);
	camera_offset = config.child("camera").attribute("offset").as_int(10);

	node = config;
	return ret;
}

bool j1Player::Start()
{
	bool ret = true;
	LOG("Player Started");
	Tex_Player = App->tex->Load("textures/test2.png");
	highlight = App->tex->GetTexture("tile_highlight", 0, 0);
	highlight_anim = App->anim->GetAnimation("tile_highlight");
	App->win->GetWindowSize(win_width, win_height);
	max_w_group = 7;
	lock_M1 = false;
	lock_M2 = false;
	SDL_ShowCursor(SDL_DISABLE);

	return ret;
}

bool j1Player::PreUpdate()
{

	return true;
}

bool j1Player::Save(pugi::xml_node& data)
{
	//PLAYER POSITION
	LOG("Loading player state");

	return true;
}

bool j1Player::Load(pugi::xml_node& data)
{
	//PLAYER POSITION
	LOG("Loading player state");

	return true;
}

bool j1Player::Update(float dt)
{
	App->input->GetMousePosition(mouse_position.x, mouse_position.y);

	if (App->scenemanager->In_Main_Menu == false) 
	{
		Camera_Control(dt);
		Camera_Limit();
	}

	if (App->clicking_ui || App->minimap->clicking_map)
		disable_click = true;
	
	if (disable_click)
	{
		if (App->input->GetMouseButtonDown(3) == KEY_REPEAT)
			lock_M2 = true;
		if (App->input->GetMouseButtonDown(1) == KEY_REPEAT)
			lock_M1 = true;
	}

	if (lock_M1)
		if (App->input->GetMouseButtonDown(1) == KEY_UP)
			lock_M1 = false;
	if (lock_M2)
		if (App->input->GetMouseButtonDown(3) == KEY_UP)
			lock_M2 = false;

	if (App->scenemanager->In_Logo_Scene != true && App->scenemanager->In_Main_Menu != true)
	if (App->input->GetMouseButtonDown(3) == KEY_DOWN && !disable_click)
	{
		iPoint m;
		App->input->GetMousePosition(m.x, m.y);
		m.x -= App->render->camera.x / App->win->GetScale();
		m.y -= App->render->camera.y / App->win->GetScale();
		iPoint tile = App->map->WorldToMap((float)m.x, (float)m.y);
		if (tile.x <= App->map->mapdata->width && tile.y <= App->map->mapdata->height && tile.x >= 0 && tile.y >= 0)
		{
			NodeType terrain = (*App->pathfinding->WorldToNode(tile.x, tile.y))->type;

			vector<fPoint> water_locations;
			vector<fPoint> ground_locations;
			vector<fPoint> air_locations;
			int water_amount = 0;
			int ground_amount = 0;
			int air_amount = 0;
			int w = 0;
			int g = 0;
			int a = 0;

			for (vector<j1Entity*>::iterator entity = App->entitymanager->selected_units.begin(); entity != App->entitymanager->selected_units.end(); entity++)
			{
				if (App->input->GetKey(SDL_SCANCODE_LSHIFT) != KEY_REPEAT)
					(*entity)->ResetPath();
				switch ((*entity)->terrain)
				{
				case NodeType::WATER:
					water_amount++;
					break;
				case NodeType::GROUND:
					ground_amount++;
					break;
				case NodeType::ALL:
					air_amount++;
					break;
				}
			}

			SpreadState state = UP;

			int i = 1;
			int i_max = 101;
			int x;
			int y;
			int loop = 0;
			int limit = water_amount + ground_amount + air_amount;
			bool last = false;
			bool scape = false;

			if (limit != 0)
				while (limit != w + g + a && scape == false)
				{
					switch (state)
					{
					case UP:
						x = tile.x + loop;
						y = tile.y - i / 2;
						if (i != 1)
							state = RIGHT;
						else
							i += 2;
						break;
					case RIGHT:
						x = tile.x + i / 2;
						y = tile.y + loop;
						state = DOWN;
						break;
					case DOWN:
						x = tile.x - loop;
						y = tile.y + i / 2;
						state = LEFT;
						break;
					case LEFT:
						x = tile.x - i / 2;
						y = tile.y - loop;
						state = UP;
						if (loop > 0)
							loop *= -1;
						else
						{
							loop *= -1;
							loop++;
						}
						if (tile.y - i / 2 == y)
						{
							loop = 0;
							i += 2;
						}
						break;
					}
					
					if (x <= App->map->mapdata->width && y <= App->map->mapdata->height && x >= 0 && y >= 0)
					{
						bool air_can = false;
						fPoint pos = pos = App->map->MapToWorld<fPoint>(x, y);
						Node* node = *App->pathfinding->WorldToNode(x, y);
						if (air_amount > a)
						{
							air_can = true;
							for (vector<fPoint>::iterator itr = air_locations.begin(); itr != air_locations.end(); itr++)
								if (*itr == pos)
								{
									air_can = false;
									break;
								}
							if (air_can)
								for (vector<j1Entity*>::iterator itr = App->entitymanager->air_units.begin(); itr != App->entitymanager->air_units.end(); itr++)
								{
									if ((*itr)->GetDestination() == pos)
									{
										air_can = false;
										break;
									}
								}
						}
						bool can = false;
						if (!node->built)
						{
							can = true;
							switch (node->type)
							{
							case NodeType::WATER:
								if (water_amount > w)
								{
									for (vector<fPoint>::iterator itr = water_locations.begin(); itr != water_locations.end(); itr++)
										if (*itr == pos)
										{
											can = false;
											break;
										}
								}
								else
									can = false;
								break;
							case NodeType::GROUND:
								if (ground_amount > g)
								{
									for (vector<fPoint>::iterator itr = ground_locations.begin(); itr != ground_locations.end(); itr++)
										if (*itr == pos)
										{
											can = false;
											break;
										}
								}
								else
									can = false;
								break;
							}
							if (can)
								for (vector<j1Entity*>::iterator e = App->entitymanager->entities.begin(); e != App->entitymanager->entities.end(); e++)
									if ((*e)->GetDestination() == pos)
									{
										can = false;
										break;
									}
						}

						if (can)
						{
							switch (node->type)
							{
							case NodeType::WATER:
								w++;
								water_locations.push_back(pos);
								break;
							case NodeType::GROUND:
								g++;
								ground_locations.push_back(pos);
								break;
							}
						}
						if (air_can)
						{
							a++;
							air_locations.push_back(pos);
						}

						if (!last)
							if(a == air_amount)
								switch (terrain)
								{
								case NodeType::WATER:
									if (w == water_amount && g != ground_amount)
									{
										i_max = i + 2 * 4;
										last = true;
									}
									break;
								case NodeType::GROUND:
									if (g == ground_amount && w != water_amount)
									{
										i_max = i + 2 * 4;
										last = true;
									}
									break;
								}
						if (i == i_max)
							scape = true;
					}
				}
			if (water_locations.size() != 0)
				std::sort(water_locations.begin(), water_locations.end(), LessY);
			if (ground_locations.size() != 0)
				std::sort(ground_locations.begin(), ground_locations.end(), LessY);
			if (air_locations.size() != 0)
				std::sort(air_locations.begin(), air_locations.end(), LessY);

			for (vector<j1Entity*>::iterator e = App->entitymanager->selected_units.begin(); e != App->entitymanager->selected_units.end(); e++)
				if ((*e)->terrain == NodeType::WATER)
				{
					if (water_locations.size() != 0)
					{
						fPoint dest = *water_locations.begin();
						(*e)->GoTo(dest, (*e)->terrain);
						water_locations.erase(water_locations.begin());
					}
					else
						(*e)->SetDestination((*e)->position);
				}
				else if ((*e)->terrain == NodeType::GROUND)
				{
					if (ground_locations.size() != 0)
					{
						fPoint dest = *ground_locations.begin();
						(*e)->GoTo(dest, (*e)->terrain);
						ground_locations.erase(ground_locations.begin());
					}
					else
						(*e)->SetDestination((*e)->position);
				}
				else if ((*e)->terrain == NodeType::ALL)
				{
					if (air_locations.size() != 0)
					{
						fPoint dest = *air_locations.begin();
						(*e)->GoTo(dest, (*e)->terrain);
						air_locations.erase(air_locations.begin());
					}
					else
						(*e)->SetDestination((*e)->position);
				}

			vector<fPoint> v;
			water_locations.swap(v);
			ground_locations.swap(v);
			air_locations.swap(v);
		}
	}
	
	dragging = false;
	//This functions should always be last//
	Mouse_Cursor(dt);
	if (!App->scenemanager->In_Main_Menu)
		if (!disable_click && dt != 0.0f)
				Drag_Mouse();

	if(!lock_M1 && !lock_M2)
		disable_click = false;

	return true;
}

bool j1Player::CleanUp()
{
	building = nullptr;
	builder = nullptr;
	return true;

}

int j1Player::Randon_Number(int minimum_value, int maximum_value)
{
	srand(time(0));
	int number = 0;
	number = rand() % maximum_value + minimum_value;
	return number;
}

void j1Player::Camera_Control(float dt)
{
	if (mouse_position.x == 0) {
		App->render->camera.x += camera_speed * dt * 1000;
	}
	if (mouse_position.y == 0) {
		App->render->camera.y += camera_speed/2 * dt * 1000;
	}

	if (mouse_position.x > (win_width - camera_offset) / App->win->scale){
			App->render->camera.x -= camera_speed*dt* 1000;
	}
		

	if (mouse_position.y > (win_height - camera_offset) / App->win->scale) {
		App->render->camera.y -= camera_speed/2 * dt * 1000;
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += camera_speed * dt * 1000;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= camera_speed * dt * 1000;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += camera_speed * dt * 1000;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= camera_speed * dt * 1000;
}

void j1Player::Select_Entitites(SDL_Rect select_area)
{
	bool single_click = false;
	for (auto entity = App->entitymanager->entities.end() - 1; entity != App->entitymanager->entities.begin() - 1; entity--)
		if (select_area.x < (*entity)->selectable_area.x + (*entity)->selectable_area.w &&
			select_area.x + select_area.w >(*entity)->selectable_area.x &&
			select_area.y < (*entity)->selectable_area.y + (*entity)->selectable_area.h &&
			select_area.h + select_area.y >(*entity)->selectable_area.y)
		{
			if (!single_click && (App->ignore_fog || App->fog->GetVisibility((*entity)->position) == FogState::VISIBLE))
			{
				if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
					(*entity)->selected = false;
				else
					(*entity)->selected = true;

				if (select_area.w * select_area.h < 10)
					single_click = true;
			}
			else
			{
				if (App->input->GetKey(SDL_SCANCODE_LSHIFT) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) != KEY_REPEAT)
					(*entity)->selected = false;
			}
		}
		else
		{
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) != KEY_REPEAT)
				(*entity)->selected = false;
		}

}

void j1Player::Mouse_Cursor(float dt) 
{
	mouse_position.x -= App->render->camera.x / App->win->GetScale();
	mouse_position.y -= App->render->camera.y / App->win->GetScale();

	iPoint pos;
	App->input->GetMousePosition(pos.x, pos.y);
	App->render->AddBlitEvent(70, Tex_Player, pos.x, pos.y, texture_rect, false, true, 0, 0, 0, 255, true);

	if (App->map->mapdata != nullptr && dt != 0.0f)
	{
		iPoint tile = App->map->WorldToMap((float)mouse_position.x, (float)mouse_position.y);
		fPoint position = App->map->MapToWorld<fPoint>(tile.x, tile.y);
		SDL_Rect r = highlight_anim.GetCurrentFrame();
		App->render->AddBlitEvent(1, highlight, position.x - 32, position.y - 32, r);
	}
}

void j1Player::Drag_Mouse()
{
	if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
	{
		App->input->GetMousePosition(start_mouse_position.x, start_mouse_position.y);
		start_mouse_position.x -= App->render->camera.x / App->win->GetScale();
		start_mouse_position.y -= App->render->camera.y / App->win->GetScale();
	}

	if (App->input->GetMouseButtonDown(1) == KEY_REPEAT)
	{
		selector = { start_mouse_position.x, start_mouse_position.y, mouse_position.x - start_mouse_position.x, mouse_position.y - start_mouse_position.y };
		if (selector.x > selector.x + selector.w)
		{
			selector.x = selector.x + selector.w;
			selector.w *= -1;
		}
		if (selector.y > selector.y + selector.h)
		{
			selector.y = selector.y + selector.h;
			selector.h *= -1;
		}

		if (selector.w * selector.h > 10 )
		{
			App->render->AddBlitEvent(4, nullptr, 0, 0, selector, false, false, 0u, 255u, 0u, 25u, true);
			dragging = true;
		}
	}

	if (App->input->GetMouseButtonDown(1) == KEY_UP)
	{
		Select_Entitites(selector);
	}

}

void j1Player::Zoom() 
{
	
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				App->win->scale = App->win->scale + 0.001;
			}
			else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				App->win->scale = App->win->scale - 0.001;
			}
		
			else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
			App->win->scale = 1.00f;
			}
}

void j1Player::Camera_Limit()
{
	//TOP CAMERA LIMITS
	if (App->render->camera.x/2 < App->render->camera.y)
		App->render->camera.y = (App->render->camera.x / 2);
	if (-App->render->camera.x/ 2 + 300< App->render->camera.y)
		App->render->camera.y = -(App->render->camera.x / 2)+300 ;

	//BOTTON CAMERA LIMITS
	if ((App->render->camera.x/2 - 6400 > App->render->camera.y)/App->win->scale)
		App->render->camera.y = ((App->render->camera.x / 2) - 6400 / App->win->scale) / App->win->scale;
	if (App->render->camera.x >= 6700 / App->win->scale)
		App->render->camera.x = 6700 / App->win->scale;
	if ((App->render->camera.x < -11700 - 2 * App->render->camera.y) / App->win->scale)
		App->render->camera.y = (-App->render->camera.x/2 - 5850 / App->win->scale) / App->win->scale;
	if (App->render->camera.x < -5850 / App->win->scale)
		App->render->camera.x = -5850 / App->win->scale;
}
