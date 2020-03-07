#include "j1App.h"
#include "j1InGameUI.h"
#include "j1Window.h"
#include "j1GUI.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Fonts.h"




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

	App->render->AddBlitEvent(2, UI_Image, 0 - App->render->camera.x, 520 - App->render->camera.y, texture_rect);
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
	menu.Return_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH-140 }, { 60,30 }, true, false, { 0,0,200,65 }, "RETURN", this);
	menu.Resume_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH -60}, { 60,30 }, true, false, { 0,0,200,65 }, "RESUME", this);
	menu.Exit_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH + 15 }, {60,30 }, true, false, { 0,0,200,65 }, "EXIT", this);
	menu.Save = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH +90 }, { 60,30 }, true, false, { 0,0,200,65 }, "SAVE", this);
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
			Activate_Menu();
		}

		if (element == menu.Exit_button) {
			Activate_Menu();
		}

		if (element == menu.Save) {
			Activate_Menu();
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