#include "j1GUIlabel.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Window.h"

j1GUIlabel::j1GUIlabel()
{
	this->type = GUItype::GUI_LABEL;
}

j1GUIlabel::~j1GUIlabel() {

}


bool j1GUIlabel::Awake(pugi::xml_node&)
{

	return true;
}

bool j1GUIlabel::Start()
{
	font_name = App->fonts->Load("textures/NameTile.png", "ABCDEFGHIJKLMNOPQRSTUWYZ0123456789-= ", 1);
	return true;
}


bool j1GUIlabel::PreUpdate()
{

	return true;
}

bool j1GUIlabel::Update(float dt)
{

	if (enabled)
		App->fonts->BlitText(Map_Position.x + Inside_Position.x - App->render->camera.x/App->win->scale, Map_Position.y + Inside_Position.y - App->render->camera.y / App->win->scale, 1, text);

	return true;
}

bool j1GUIlabel::PostUpdate()
{

	return true;
}

bool j1GUIlabel::CleanUp()
{
	App->tex->UnLoad(texture);
	return true;
}