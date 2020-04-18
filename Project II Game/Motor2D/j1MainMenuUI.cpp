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
#include "j1TransitionManager.h"
#include <vector>
#include <iostream>

#include "j1Scene.h"


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
	menu.music->Button->map_position.x = 830 + 186;
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
	menu.link = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 1220,660 }, { 0,0 }, true, false, { 0,0,60,60 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::LINK);
	//AUDIO OPTIONS
	menu.music = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { 830, 230 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::SCROLL);
	menu.music_label = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 850,200 }, { 0,0 }, true, false, { 0,0,40,40 }, "MUSIC", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	menu.fx = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { 830, 290 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::SCROLL);
	menu.fx_label = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 850,260 }, { 0,0 }, true, false, { 0,0,40,40 }, "EFFECTS", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	menu.audio_image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, {  800,170 }, { 0,0 }, true, false, { 0, 0,300,200 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::AUDIO_IMAGE);
}




void j1MainMenuUI::Activate_Menu()
{
	menu.start->enabled = true;
	menu.audio_button->enabled = true;
	menu.quit->enabled = true;
	menu.fullscreen->enabled = true;
	menu.link->enabled = true;
}

void j1MainMenuUI::Deactivate_Menu()
{
	menu.start->enabled = false;
	menu.audio_button->enabled = false;
	menu.quit->enabled = false;
	menu.fullscreen->enabled = false;
	menu.link->enabled = false;
}

void j1MainMenuUI::Activate_Audio_Options()
{
	menu.music->enabled = true;
	menu.music_label->enabled = true;
	menu.fx->enabled = true;
	menu.fx_label->enabled = true;
	menu.audio_image->enabled = true;
	audioopt = true;
}

void j1MainMenuUI::Deactivate_Audio_Options()
{
	menu.music->enabled = false;
	menu.music_label->enabled = false;
	menu.fx->enabled = false;
	menu.fx_label->enabled = false;
	menu.audio_image->enabled = false;
	audioopt = false;
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
			App->transitions->LinesAppearing(Black, 0.75f, 1);
			App->scene->start = true;
			Deactivate_Audio_Options();
			App->game_pause = false;
		}
			
		if (element == menu.audio_button) 
		{
			if (audioopt == false)
				Activate_Audio_Options();
			else
				Deactivate_Audio_Options();
		}

		if (element == menu.fullscreen)
		{
			App->win->Fullscreen();
		}

		if (element == menu.quit) 
		{
			Deactivate_Audio_Options();
			quit = false;
		}
		if (element == menu.link)
		{
			ShellExecuteA(NULL, "open", "https://www.youtube.com/watch?v=dQw4w9WgXcQ", NULL, NULL, SW_SHOWNORMAL);
		}
		
	}
	}
}

bool j1MainMenuUI::PostUpdate()
{
	return quit;
}

Main_Menu j1MainMenuUI::GetMenu()
{
	return menu;
}