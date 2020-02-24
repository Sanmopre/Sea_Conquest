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
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		App->input->GetMousePosition(mouse_position2.x, mouse_position2.y);
	}
	return true;
}

bool j1Player::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
		App->input->GetMousePosition(mouse_position.x, mouse_position.y);
		prueba = { mouse_position2.x,mouse_position2.y,mouse_position.x - mouse_position2.x,mouse_position.y - mouse_position2.y };
		App->render->DrawQuad(prueba, 0, 255, 0, 80);
	}

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

bool j1Player::CleanUp() {

	return true;

}


