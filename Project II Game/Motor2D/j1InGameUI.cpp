#include "j1App.h"
#include "j1InGameUI.h"
#include "j1Window.h"
#include "j1GUI.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Input.h"
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
	Add_UI();
	return true;
}

bool j1InGameUI::PreUpdate()
{

	return true;
}


bool j1InGameUI::Update(float dt)
{

	App->render->AddBlitEvent(2, UI_Image, 0 - App->render->camera.x / App->win->scale, 520 - App->render->camera.y / App->win->scale, texture_rect);
	App->render->AddBlitEvent(3,resources , 0 - App->render->camera.x / App->win->scale, 0 - App->render->camera.y / App->win->scale, texture_rect_1);





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
	menu.Menu_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, {width -250,10 }, { 60,30 }, true, true, { 0,0,200,65 }, "OPTIONS", this);
	menu.Return_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH-140 }, { 0,30 }, true, false, { 0,0,200,65 }, "MORE RESOURCES", this);
	menu.Resume_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH -60}, { 60,30 }, true, false, { 0,0,200,65 }, "RESUME", this);
	menu.Exit_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH + 15 }, {60,30 }, true, false, { 0,0,200,65 }, "FULLSCREEN", this);
	menu.Save = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH +90 }, { 60,30 }, true, false, { 0,0,200,65 }, "QUIT", this);
	menu.Load = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH +165}, { 60,30 }, true, false, { 0,0,200,65 }, "LOAD", this);
	menu.Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { MiddleScreenW - 50,0 }, { 0,0 }, true, false, { 0, 0,350,500 },"",this);
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

void j1InGameUI::GUI_Event_Manager(GUI_Event type, j1GUIelement* element)
{
	switch (type)
	{

	case GUI_Event::EVENT_ONCLICK:
	{

		if (element == menu.Return_button) {
			//Activate_Menu();
			type_1 = type_1 + 100;
			type_2 = type_2 + 420;
			type_0 = type_0 + 69;
		}

		if (element == menu.Exit_button) {
			Activate_Menu();
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