#include "p2Defs.h"
#include "j1App.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1Window.h"

#include "j1EntityManager.h"



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

	camera_speed = config.child("camera").attribute("speed").as_int(1);
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
	return ret;

}

bool j1Player::PreUpdate()
{

	return true;
}

bool j1Player::Update()
{
	Camera_Control();
	Drag_Mouse();
	Mouse_Cursor();
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

bool j1Player::CleanUp()
{

	return true;

}

void j1Player::Drag_Mouse() 
{

	if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
	{
		App->input->GetMousePosition(start_mouse_position.x, start_mouse_position.y);
		start_mouse_position.x -= App->render->camera.x;
		start_mouse_position.y -= App->render->camera.y;
	}

	if (App->input->GetMouseButtonDown(1) == KEY_REPEAT)
	{
		App->input->GetMousePosition(mouse_position.x, mouse_position.y);
		mouse_position.x -= App->render->camera.x;
		mouse_position.y -= App->render->camera.y;
		selector = { start_mouse_position.x, start_mouse_position.y, mouse_position.x - start_mouse_position.x, mouse_position.y - start_mouse_position.y };
		App->render->DrawQuad(selector,0,255,0,25);
	}

	if (App->input->GetMouseButtonDown(1) == KEY_UP)
	{
		Select_Entitites(selector);
	}

}

void j1Player::Camera_Control()
{
	App->input->GetMousePosition(mouse_position.x, mouse_position.y);
	if (mouse_position.x == 0) {
		App->render->camera.x = App->render->camera.x + camera_speed;
	}
	if (mouse_position.y == 0) {
		App->render->camera.y = App->render->camera.y + camera_speed;
	}
	if (mouse_position.x > win_width - camera_offset) {
		App->render->camera.x = App->render->camera.x - camera_speed;
	}
	if (mouse_position.y > win_height - camera_offset) {
		App->render->camera.y = App->render->camera.y - camera_speed;
	}
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
		if ((*entity)->position.x > select_area.x &&
			(*entity)->position.x < (select_area.x + select_area.w) &&
			(*entity)->position.y > select_area.y &&
			(*entity)->position.y < (select_area.y + select_area.h))
			(*entity)->selected = true;
		else
			(*entity)->selected = false;	
}

void j1Player::Mouse_Cursor() 
{
	SDL_ShowCursor(SDL_DISABLE);
	App->input->GetMousePosition(mouse_poisition_static.x, mouse_poisition_static.y);
	App->render->Blit(Tex_Player, mouse_poisition_static.x, mouse_poisition_static.y, &texture_rect, SDL_FLIP_NONE, 1.0);
}