#include "j1GUIElements.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1GUI.h"


j1Image::j1Image() {

	this->type = GUItype::GUI_IMAGE;
}

j1Image::~j1Image() {

}

bool j1Image::Start()
{
	if (textureType == TEXTURE::IMAGE) 
		texture = App->gui->Load_Texture(TEXTURE::IMAGE);

	if (textureType == TEXTURE::MANAGER_IMAGE) 
		texture = App->gui->Load_Texture(TEXTURE::MANAGER_IMAGE);

	if (textureType == TEXTURE::MAIN_IMAGE)
		texture = App->gui->Load_Texture(TEXTURE::MAIN_IMAGE);

	if (textureType == TEXTURE::RESOURCES_IMAGE)
		texture = App->gui->Load_Texture(TEXTURE::RESOURCES_IMAGE);

	if (textureType == TEXTURE::BOAT_IMAGE)
		texture = App->gui->Load_Texture(TEXTURE::BOAT_IMAGE);

	if (textureType == TEXTURE::BUILDING_IMAGE)
		texture = App->gui->Load_Texture(TEXTURE::BUILDING_IMAGE);

	if (textureType == TEXTURE::HARVESTER)
		texture = App->gui->Load_Texture(TEXTURE::HARVESTER);

	if(textureType == TEXTURE::AUDIO_IMAGE)
		texture = App->gui->Load_Texture(TEXTURE::AUDIO_IMAGE);

	if (textureType == TEXTURE::TOWNHALL)
		texture = App->gui->Load_Texture(TEXTURE::TOWNHALL);

	if (textureType == TEXTURE::STORAGE)
		texture = App->gui->Load_Texture(TEXTURE::STORAGE);

	if (textureType == TEXTURE::COST)
		texture = App->gui->Load_Texture(TEXTURE::COST);

	if (textureType == TEXTURE::GODMODE)
		texture = App->gui->Load_Texture(TEXTURE::GODMODE);

	if (textureType == TEXTURE::INFO_IMAGE)
		texture = App->gui->Load_Texture(TEXTURE::INFO_IMAGE);

	return true;
}

bool j1Image::PreUpdate()
{

	return true;
}

bool j1Image::Update(float dt) {


	if (enabled) {
		if (textureType == TEXTURE::MAIN_IMAGE)
		App->render->AddBlitEvent(3, texture, map_position.x - App->render->camera.x, map_position.y - App->render->camera.y, rect, false,true, 0, 0, 0, 0, true);
		else if(textureType == TEXTURE::AUDIO_IMAGE)
		App->render->AddBlitEvent(3, texture, map_position.x - App->render->camera.x, map_position.y - App->render->camera.y, rect, false, true, 0, 0, 0, 0, true);
		else
		App->render->AddBlitEvent(4, texture, map_position.x - App->render->camera.x, map_position.y - App->render->camera.y, rect, false, true, 0, 0, 0, 0, true);
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

