#include "j1App.h"
#include "j1InGameUI.h"
#include "j1Window.h"
#include "j1GUI.h"




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

	return true;
}

bool j1InGameUI::CleanUp()
{

	return true;

}

//UI FUNCTIONS
void j1InGameUI::Add_UI()
{
	menu.Menu_button = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, {width - 200,20 }, { 0,0 }, true, true, { 0,0,100,50 }, "OPTIONS", this);
	menu.Return_button = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW,MiddleScreenH }, { 00,0 }, true, false, { 0, 0,130,22 }, "RETURN", this);
	menu.Title = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW - 20,MiddleScreenH-140}, { 0,0 }, false, false, { 0, 0,175,47 }, "MENU", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	menu.Resume_button = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW,MiddleScreenH + 30 }, { 0,0 }, true, false, { 0, 0,130,22 }, "RESUME", this);
	menu.Exit_button = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW,MiddleScreenH +60 }, { 0,0 }, true, false, { 0, 0,130,22 }, "EXIT", this);
	menu.Save = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW,MiddleScreenH +90 }, { 0,0 }, true, false, { 0, 0,130,22 }, "SAVE", this);
	menu.Load = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW,MiddleScreenH +120 }, { 0,0 }, true, false, { 0, 0,130,22 }, "LOAD", this);
	menu.Image = App->gui->ADD_ELEMENT(GUItype::GUI_IMAGE, nullptr, { MiddleScreenW - 50,MiddleScreenH - 140 }, { 0,0 }, true, false, { 0, 0,245,300 },"",this);
}


void j1InGameUI::Activate_Menu()
{
	menu.Resume_button->enabled = !menu.Resume_button->enabled;
	menu.Return_button->enabled = !menu.Return_button->enabled;
	menu.Title->enabled = !menu.Title->enabled;
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

		if (element == menu.Exit_button) {
			Activate_Menu();
		}
	}
	}
}