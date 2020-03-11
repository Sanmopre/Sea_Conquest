#include "j1GUIElements.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"


j1Image::j1Image() {

	this->type = GUItype::GUI_IMAGE;
}

j1Image::~j1Image() {

}

bool j1Image::Start()
{
	menu_image = App->tex->Load("textures/image.png");
	return true;
}

bool j1Image::PreUpdate()
{

	return true;
}

bool j1Image::Update(float dt) {


	if (enabled) {
			App->render->AddBlitEvent(3, menu_image, map_position.x - App->render->camera.x / App->win->scale, map_position.y - App->render->camera.y / App->win->scale, rect);
	}



return true; 
}

bool j1Image::PostUpdate()
{
	if (enabled)
		Draw();

	return true;
}

bool j1Image::CleanUp()
{
	return true;
}

