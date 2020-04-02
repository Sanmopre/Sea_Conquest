#include "j1App.h"
#include "j1MainMenuUI.h"
#include "j1Window.h"
#include "j1GUI.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "j1Entities.h"
#include "j1SceneManager.h"
#include <vector>
#include <iostream>




j1MainMenuUI::j1MainMenuUI() : j1Module()
{
	name.create("InGameUI");
}


j1MainMenuUI ::~j1MainMenuUI()
{

}

bool j1MainMenuUI::Awake(pugi::xml_node& config)
{

	return true;
}


bool j1MainMenuUI::Start()
{

	Add_UI();

	return true;
}

bool j1MainMenuUI::PreUpdate()
{

	return true;
}


bool j1MainMenuUI::Update(float dt)
{

	return true;


}




//UI FUNCTIONS
void j1MainMenuUI::Add_UI()
{
	//MENU
	menu.start = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 510,140 }, { 50,25 }, true, false, { 0,0,200,65 }, "START", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);
	menu.audio_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 510, 240}, { 30,25 }, true, false, { 0,0,200,65 }, "AUDIO OPT", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);
	menu.quit = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 510,440 }, { 50,25 }, true, false, { 0,0,200,65 }, "QUIT", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);
	menu.fullscreen = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 510,340 }, { 50,25 }, true, false, { 0,0,200,65 }, "FULLSCREEN", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);
}




void j1MainMenuUI::Activate_Menu()
{
	menu.start->enabled = true;
	menu.audio_button->enabled = true;
	menu.quit->enabled = true;
	menu.fullscreen->enabled = true;
}

void j1MainMenuUI::Deactivate_Menu()
{
	menu.start->enabled = false;
	menu.audio_button->enabled = false;
	menu.quit->enabled = false;
	menu.fullscreen->enabled = false;
}

void j1MainMenuUI::GUI_Event_Manager(GUI_Event type, j1Element* element)
{
	clicking_ui = true;
	switch (type)
	{
	case GUI_Event::EVENT_ONCLICK:
	{
		if (element == menu.start) 
		{
			App->scenemanager->ChangeScene(1);
		}
			
		if (element == menu.audio_button) 
		{
		
		}

		if (element == menu.fullscreen)
		{
			App->win->Fullscreen();
		}

		if (element == menu.quit) 
		{
			quit = false;
		}
		
	}
	}
}

bool j1MainMenuUI::PostUpdate()
{
	return quit;
}