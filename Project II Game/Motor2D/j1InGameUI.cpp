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
	MiddleScreen = App->win->width/2 - 100;
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
	menu.Menu_button = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, { 1400,20 }, { 0,0 }, true, true, { 0,0,100,50 }, "OPTIONS", this);
	menu.Return_button = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, { MiddleScreen,430 }, { 00,0 }, true, false, { 0, 0,130,22 }, "RETURN", this);
	menu.Title = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, { MiddleScreen,300 }, { 0,0 }, false, false, { 0, 0,139,27 }, "MENU", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	menu.Resume_button = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, { MiddleScreen,400 }, { 0,0 }, true, false, { 0, 0,130,22 }, "RESUME", this);
	menu.Exit_button = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, { MiddleScreen,460 }, { 0,0 }, true, false, { 0, 0,130,22 }, "EXIT", this);
	menu.Save = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, { MiddleScreen,520 }, { 0,0 }, true, false, { 0, 0,130,22 }, "SAVE", this);
	menu.Load = App->gui->ADD_ELEMENT(GUItype::GUI_BUTTON, nullptr, { MiddleScreen,490 }, { 0,0 }, true, false, { 0, 0,130,22 }, "LOAD", this);
}


void j1InGameUI::Activate_Menu()
{
	menu.Resume_button->enabled = !menu.Resume_button->enabled;
	menu.Return_button->enabled = !menu.Return_button->enabled;
	menu.Title->enabled = !menu.Title->enabled;
	menu.Exit_button->enabled = !menu.Exit_button->enabled;
	menu.Load->enabled = !menu.Load->enabled;
	menu.Save->enabled = !menu.Save->enabled;
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