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


	node = config;
	return ret;
}


bool j1Player::Start()
{
	bool ret = true;
	LOG("Player Started");
	Tex_Player = App->tex->Load("");
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

}

void j1Player::Camera_Control() {

	App->input->GetMousePosition(mouse_position.x, mouse_position.y);
	if (mouse_position.x == 0) {
		App->render->camera.x = App->render->camera.x + camera_speed;
	}
	if (mouse_position.y == 0) {
		App->render->camera.y = App->render->camera.y + camera_speed;
	}
	if (mouse_position.x > 1915) {
		App->render->camera.x = App->render->camera.x - camera_speed;
	}
	if (mouse_position.y > 1075) {
		App->render->camera.y = App->render->camera.y - camera_speed;
	}

}
