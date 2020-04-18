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

#include "j1EntityManager.h"

#define X_DISTANCE 30
#define Y_DISTANCE 30

using namespace std;

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
	App->win->GetWindowSize( win_width,win_height);
	max_w_group = 7;
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
	mouse_position.x = data.child("position").attribute("X").as_int();
	mouse_position.y = data.child("position").attribute("Y").as_int();
	return true;
}

bool j1Player::Load(pugi::xml_node& data)
{
	//PLAYER POSITION
	LOG("Loading player state");
	mouse_position.x = data.child("position").attribute("X").as_int();
	mouse_position.y = data.child("position").attribute("Y").as_int();
	return true;
}

bool j1Player::Update(float dt)
{
	App->input->GetMousePosition(mouse_position.x, mouse_position.y);
	if (App->scenemanager->In_Main_Menu == false) {
		Camera_Control(dt);
		Zoom();
		Camera_Limit();
	}


	if (App->input->GetMouseButtonDown(3) == KEY_DOWN && !disable_click)
	{
		iPoint m;
		App->input->GetMousePosition(m.x, m.y);
		m.x -= App->render->camera.x / App->win->GetScale();
		m.y -= App->render->camera.y / App->win->GetScale();
		if (App->entitymanager->selected_n > 1)
		{
			int n = 0;
			int w_group = 0;
			fPoint x = { 0,0 };
			fPoint y = { 0,0 };
			fPoint w, h;
			for (vector<j1Entity*>::iterator i = App->entitymanager->entities.begin(); i != App->entitymanager->entities.end(); i++)
			{
				if ((*i)->selected && (*i)->main_type == EntityType::UNIT && (*i)->team == 1)
				{
					if (n == 0)
					{
						//direction detector
						fPoint v = { (*i)->position.x - m.x , (*i)->position.y - m.y };
						float m = v.y / v.x;
						h.x = Y_DISTANCE / sqrt(m * m + 1);
						h.y = m * h.x;

						if (v.x < 0)
							h.Negate();

						v = { -v.y, v.x };
						m = v.y / v.x;
						w.x = X_DISTANCE / sqrt(m * m + 1);
						w.y = m * w.x;
					}
					if (w_group == max_w_group - 1)
					{
						x.SetToZero();
						y += h;
						w_group = 0;
					}
					(*i)->GoTo({ (float)m.x + x.x + y.x, (float)m.y + x.y + y.y }, (*i)->terrain);
					if (n % 2 == 0)
					{
						x.Negate();
						x += w;
					}
					else
						x.Negate();
					n++;
					w_group++;
				}
				if (n == App->entitymanager->selected_n)
					break;
			}
		}

	}
	
	//This functions should always be last//
	Mouse_Cursor();
	if (App->scenemanager->In_Main_Menu == false) {
		if (App->InGameUI->clicking_ui == false && !disable_click && App->minimap->clicking_map == false)
			if (dt != 0.0f)
				Drag_Mouse();
	}

	disable_click = false;


//////////////////////////////////////

	if (App->scenemanager->In_Logo_Scene != true && App->scenemanager->In_Main_Menu != true) {
		App->render->AddBlitEvent(5, App->minimap->minimap_tex, App->minimap->position.x - App->render->camera.x, App->minimap->position.y - App->render->camera.y, App->minimap->rect, false, true, 0u, 0u, 0u, 255, true);
	    App->minimap->DrawCamera();
	}
///////////////////////////////
	return true;
}

bool j1Player::CleanUp()
{

	return true;

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
	int buffer;
	if (select_area.x > select_area.x + select_area.w)
	{
		select_area.x = select_area.x + select_area.w;
		select_area.w *= -1;
	}
	if (select_area.y > select_area.y + select_area.h)
	{
		select_area.y = select_area.y + select_area.h;
		select_area.h *= -1;
	}
	//LOG("Ax -> %d | Ay -> %d | Aw -> %d | Ah -> %d", select_area.x, select_area.y, select_area.w, select_area.h);

	for (auto entity = App->entitymanager->entities.begin(); entity != App->entitymanager->entities.end(); entity++)
		if (select_area.x < (*entity)->GetRenderPositionX() + (*entity)->rect.w &&
			select_area.x + select_area.w >(*entity)->GetRenderPositionX() &&
			select_area.y < (*entity)->GetRenderPositionY() + (*entity)->rect.h &
			select_area.h + select_area.y >(*entity)->GetRenderPositionY())
			if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
				(*entity)->selected = false;
			else
				(*entity)->selected = true;
		else
		{
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) != KEY_REPEAT)
				(*entity)->selected = false;
		}
}

void j1Player::Mouse_Cursor() 
{
	mouse_position.x -= App->render->camera.x / App->win->GetScale();
	mouse_position.y -= App->render->camera.y / App->win->GetScale();
	App->render->AddBlitEvent(7,Tex_Player, mouse_position.x, mouse_position.y, texture_rect);
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
		App->render->AddBlitEvent(2, nullptr, 0,0, selector, false, false, 0u, 255u, 0u, 25u, true);
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
	if (App->render->camera.x/2 - 6400 > App->render->camera.y)
		App->render->camera.y = (App->render->camera.x / 2) - 6400;
	if (App->render->camera.x >= 6700)
		App->render->camera.x = 6700;
	if (App->render->camera.x < -11700 - 2 * App->render->camera.y) 
		App->render->camera.y = -App->render->camera.x/2 - 5850;
	if (App->render->camera.x < -5850)
		App->render->camera.x = -5850;
}
