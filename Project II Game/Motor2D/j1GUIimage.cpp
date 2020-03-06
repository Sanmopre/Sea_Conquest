#include "j1GUIimage.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"


j1GUIimage::j1GUIimage() {

	this->type = GUItype::GUI_IMAGE;
}

j1GUIimage::~j1GUIimage() {

}

bool j1GUIimage::Start()
{
	menu_image = App->tex->Load("textures/image.png");
	return true;
}

bool j1GUIimage::Awake(pugi::xml_node&)
{

	return true;
}

bool j1GUIimage::PreUpdate()
{

	return true;
}

bool j1GUIimage::Update(float dt) { 


	if (enabled) {
			App->render->AddBlitEvent(3, menu_image, Map_Position.x - App->render->camera.x, Map_Position.y - App->render->camera.y, rect);
	}



return true; 
}

bool j1GUIimage::PostUpdate()
{
	if (enabled)
		Draw();

	return true;
}

bool j1GUIimage::CleanUp()
{
	return true;
}

