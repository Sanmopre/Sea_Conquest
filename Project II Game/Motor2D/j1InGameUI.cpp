#include "j1App.h"
#include "j1InGameUI.h"
#include "j1Window.h"
#include "j1GUI.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "j1Entities.h"
#include <vector>
#include <iostream>




j1InGameUI::j1InGameUI() : j1Module()
{

	name.create("InGameUI");
}


j1InGameUI::~j1InGameUI()
{

}

bool j1InGameUI::Awake(pugi::xml_node& config)
{

	return true;
}


bool j1InGameUI::Start()
{

	type_0 = 0;
	type_1 = 0;
	type_2 = 0;

	font_name = App->fonts->Load("textures/NameTile.png", "ABCDEFGHIJKLMNOPQRSTUWYZ0123456789-= ", 1);

	resources = App->tex->Load("textures/Recursos.png");
	UI_Image = App->tex->Load("textures/UIimage.png");
	MiddleScreenW = App->win->width/2 - 100;
	MiddleScreenH = App->win->height/ 2 - 100;
	width = App->win->width;



	//CREATES UI

	Add_UI();


	//DISABLES UNITS UI
	building.Boat_Building_Button->enabled = false;
	return true;
}

bool j1InGameUI::PreUpdate()
{

	return true;
}


bool j1InGameUI::Update(float dt)
{

	//App->render->AddBlitEvent(2, UI_Image, 0 - App->render->camera.x / App->win->scale, 520 - App->render->camera.y / App->win->scale, texture_rect);
	//App->render->AddBlitEvent(3,resources , 0 - App->render->camera.x / App->win->scale, 0 - App->render->camera.y / App->win->scale, texture_rect_1);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//SEARCHING FOR SELECTED LOOP 
	bool found_boat_builder = false;
	bool found = false;
	for (std::vector<j1Entity*>::iterator entity = App->entitymanager->entities.begin(); entity != App->entitymanager->entities.end(); entity++)
	{
		if ((*entity)->selected)
		{
			Activate_Manager();
			found = true;
			if ((*entity)->type == EntityType::BOATHOUSE) {
				{
					entity_ui = *entity;
					Activate_Building_Menu();
					found_boat_builder = true;
					break;
				}
			}
			
		}
	}

	if (found_boat_builder == false)
	Deactivate_Building_Menu();

	if (found == false)
	Deactivate_Manager();
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	



	//UPDATE RESOURCES

	sprintf_s(text_type_0, 10, "%7d", type_0);
	sprintf_s(text_type_1, 10, "%7d", type_1);
	sprintf_s(text_type_2, 10, "%7d", type_2);

	App->fonts->BlitText(10 - App->render->camera.x / App->win->scale, 5 - App->render->camera.y / App->win->scale, 1,text_type_0);
	App->fonts->BlitText(140 - App->render->camera.x / App->win->scale, 5 - App->render->camera.y / App->win->scale, 1, text_type_1);
	App->fonts->BlitText(280 - App->render->camera.x / App->win->scale, 5 - App->render->camera.y / App->win->scale, 1, text_type_2);

	//MENU FROM ESC

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		Activate_Menu();
	return true;


}

bool j1InGameUI::CleanUp()
{

	return true;

}

//UI FUNCTIONS
void j1InGameUI::Add_UI()
{
	//MENU
	menu.Menu_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, {width -50,10 }, { 0,0 }, true, true, { 0,0,40,40 }, "", this,true,true, SCROLL_TYPE::SCROLL_NONE,true, TEXTURE::OPTIONS);
	menu.Return_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH-140 }, { 0,30 }, true, false, { 0,0,200,65 }, "MORE RESOURCES", this, true, true, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);
	menu.Resume_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH -60}, { 60,30 }, true, false, { 0,0,200,65 }, "RESUME", this, true, true, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);
	menu.Exit_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH + 15 }, {60,30 }, true, false, { 0,0,200,65 }, "FULLSCREEN", this, true, true, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);
	menu.Save = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH +90 }, { 60,30 }, true, false, { 0,0,200,65 }, "QUIT", this, true, true, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);
	menu.Load = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH +165}, { 60,30 }, true, false, { 0,0,200,65 }, "LOAD", this, true, true, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);
	menu.Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { MiddleScreenW - 50,0 }, { 0,0 }, true, false, { 0, 0,350,500 },"", this, true, true, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::IMAGE);

	//ENTITY_MANAGER_UI
	manager.image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { MiddleScreenW - 390,525 }, { 0,0 }, true, false, { 0, 0,350,170 }, "", this, true, true, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::MANAGER_IMAGE);
	manager.button_next = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW - 440,585 }, { 0,0 }, true, true, { 0,0,40,40 }, "", this, true, true, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::NEXT);
	manager.buton_prev = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW -25,585 }, { 0,0 }, true, true, { 0,0,40,40 }, "", this, true, true, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::NEXT);
	manager.entity_type_Image = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { width - 50,10 }, { 0,0 }, true, true, { 0,0,40,40 }, "", this, true, true, SCROLL_TYPE::SCROLL_NONE, true);
	manager.entity_name_boat = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 205,550 }, { 0,0 }, true, true, { 0,0,40,40 }, "BOAT", this, true, true, SCROLL_TYPE::SCROLL_NONE, true);
//	manager.entity_name_boathouse = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 205,550 }, { 0,0 }, true, true, { 0,0,40,40 }, "BOAT HOUSE", this, true, true, SCROLL_TYPE::SCROLL_NONE, true);


	//BOAT_BUILDER_MENU
	building.Boat_Building_Button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 195,600 }, { 20,30 }, true, true, { 0,0,200,65 }, "CREATE BOAT", this, true, true, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);


	//UI BASICS ALWAYS ACTIVE
	basics.Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, {0,520 }, { 0,0 }, true, true, { 0, 0,1280,200 }, "", this, true, true, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::MAIN_IMAGE);
	basics.Resources = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, {0,0 }, { 0,0 }, true, true, { 0, 0,400,30 }, "", this, true, true, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::RESOURCES_IMAGE);
}


void j1InGameUI::Activate_Menu()
{
	menu.Resume_button->enabled = !menu.Resume_button->enabled;
	menu.Return_button->enabled = !menu.Return_button->enabled;
	menu.Exit_button->enabled = !menu.Exit_button->enabled;
	menu.Load->enabled = !menu.Load->enabled;
	menu.Save->enabled = !menu.Save->enabled;
	menu.Image->enabled = !menu.Image->enabled;
}

void j1InGameUI::GUI_Event_Manager(GUI_Event type, j1Element* element)
{
	switch (type)
	{

	case GUI_Event::EVENT_ONCLICK:
	{

		if (element == menu.Return_button)
			for (std::vector<j1Entity*>::iterator entity = App->entitymanager->entities.begin(); entity != App->entitymanager->entities.end(); entity++)
			{
				if ((*entity)->selected)
				{
					(*entity)->storage.cotton += 10;
					(*entity)->storage.wood += 15;
					(*entity)->storage.metal += 5;
				}
			}

		if (element == menu.Exit_button) {
			App->win->Fullscreen();
		}

		if (element == menu.Save) {
			quit = true;
		}
		if (element == menu.Load) {
			Activate_Menu();
		}

		if (element == menu.Resume_button) {
			Activate_Menu();
		}
		if (element == menu.Menu_button) {
			Activate_Menu();
		}
		if (element == building.Boat_Building_Button) {
			entity_ui->BuildUnit(EntityType::BOAT,0);
		}

	}
	}
}

bool j1InGameUI::PostUpdate()
{
	bool ret = true;
	if (quit == true) {
		return false;
	}
	return ret;

}


void j1InGameUI::Activate_Building_Menu()
{
	building.Boat_Building_Button->enabled = true;
}

void j1InGameUI::Deactivate_Building_Menu() 
{
	building.Boat_Building_Button->enabled = false;
}


void j1InGameUI::Activate_Manager()
{
	manager.button_next->enabled = true;
	manager.buton_prev->enabled = true;
	manager.image->enabled = true;
    manager.entity_type_Image->enabled = true;
	manager.entity_name_boat->enabled = true;
}

void j1InGameUI::Deactivate_Manager()
{
	manager.button_next->enabled = false;
	manager.buton_prev->enabled = false;
	manager.image->enabled = false;
	manager.entity_type_Image->enabled = false;
	manager.entity_name_boat->enabled = false;
}

