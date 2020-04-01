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
#include "j1SceneManager.h"
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

	MiddleScreenW = App->win->width/2 - 100;
	MiddleScreenH = App->win->height/ 2 - 100;
	width = App->win->width;

	selected_offset = 0;
	selected_total = 0;

	//CREATES UI

	Add_UI();

	return true;
}

bool j1InGameUI::PreUpdate()
{

	return true;
}

bool j1InGameUI::Update(float dt)
{
	GetSelectedEntity();
	if (App->scenemanager->In_Main_Menu == false && selected_total != 0) {
		Manage_Entity_UI(selected);
	}
	else {
		in_trading = false;
		Manage_Entity_UI(nullptr);
	}
	
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN) {
	
	}

	//UPDATE RESOURCES
	sprintf_s(text_type_0, 10, "%7d", menu.Scroll->Value);
	sprintf_s(text_type_1, 10, "%7d", type_1);
	sprintf_s(text_type_2, 10, "%7d", type_2);
	if (App->scenemanager->In_Main_Menu == false) {
		App->fonts->BlitText(10, 5, 1, text_type_0);
		App->fonts->BlitText(140, 5, 1, text_type_1);
		App->fonts->BlitText(280, 5, 1, text_type_2);
	}

	//MENU FROM ESC
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		Activate_Menu();
		App->game_pause = !App->game_pause;
	}


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
	menu.Menu_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, {width -50,10 }, { 0,0 }, true, true, { 0,0,40,40 }, "", this,false, false, SCROLL_TYPE::SCROLL_NONE,true, TEXTURE::OPTIONS);
	menu.Return_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH-140 }, { 0,30 }, true, false, { 0,0,200,65 }, "MORE RESOURCES", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);
	menu.Resume_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH -60}, { 60,30 }, true, false, { 0,0,200,65 }, "RESUME", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);
	menu.Exit_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH + 15 }, {60,30 }, true, false, { 0,0,200,65 }, "FULLSCREEN", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);
	menu.Save = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH +90 }, { 60,30 }, true, false, { 0,0,200,65 }, "QUIT", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);
	menu.Load = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH +165}, { 35,30 }, true, false, { 0,0,200,65 }, "MAIN MENU", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);
	menu.Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { MiddleScreenW - 50,0 }, { 0,0 }, true, false, { 0, 0,350,500 },"", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::IMAGE);



	///////////////////////////////
	menu.Scroll = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { 190, 70 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::SCROLL);
	///////////////////////////////
	
	


	//ENTITY_MANAGER_UI
	manager.image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { MiddleScreenW - 390,525 }, { 0,0 }, true, false, { 0, 0,350,170 }, "", this,false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::MANAGER_IMAGE);
	manager.button_next = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW - 440,585 }, { 0,0 }, true, true, { 0,0,40,40 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::PREV);
	manager.buton_prev = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW -25,585 }, { 0,0 }, true, true, { 0,0,40,40 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::NEXT);
	
	
	//BOAT_MENU
	boat.entity_name_boat = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 245,555 }, { 0,0 }, true, true, { 0,0,40,40 }, "BOAT ", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	boat.entity_type_Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 205,550 }, { 0,0 }, true, false, { 0, 0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BOAT_IMAGE);
	boat.Trade = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 195,645 }, { 0,0 }, true, true, { 0,0,30,30 }, nullptr, this, true, true, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADE);


	//HARVESTER
	harvester.entity_name_Harvester = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 245,555 }, { 0,0 }, true, true, { 0,0,40,40 }, "HARVESTER", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	harvester.entity_type_Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 205,550 }, { 0,0 }, true, false, { 0, 0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::HARVESTER);
	harvester.Trade = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 195,645 }, { 0,0 }, true, true, { 0,0,30,30 }, nullptr, this, true, true, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADE);


	//BOAT_BUILDER_MENU
	building.Boat_Building_Button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 195,600 }, { 0,0 }, true, true, { 0,0,30,30 }, "10", this, true, true, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BOAT_IMAGE);
	building.entity_name_boathouse = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 245,555 }, { 0,0 }, true, true, { 0,0,40,40 }, "BOAT HOUSE", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	building.entity_type_Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 205,550 }, { 0,0 }, true, false, { 0, 0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUILDING_IMAGE);
	building.Trade = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 195,645 }, { 0,0 }, true, true, { 0,0,30,30 }, nullptr, this, true, true, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADE);

	//UI BASICS ALWAYS ACTIVE
	basics.Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, {0,520 }, { 0,0 }, true, true, { 0, 0,1280,200 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::MAIN_IMAGE);
	basics.Resources = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, {0,0 }, { 0,0 }, true, true, { 0, 0,400,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::RESOURCES_IMAGE);


	//TRADING_MENU
	trading.Scroll = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { 190, 560 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::SCROLL);
	trading.Scroll_1 = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { 190, 590 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::SCROLL);
	trading.Scroll_2 = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { 190, 620 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::SCROLL);
	trading.back = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 195,645 }, { 0,0 }, true, true, { 0,0,30,30 }, nullptr, this, true, true, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADE);

	//TRADER 
	trader.image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { MiddleScreenW + 80,525 }, { 0,0 }, true, false, { 0, 0,350,170 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::MANAGER_IMAGE);
	trader.buton_prev = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 430 ,585 }, { 0,0 }, true, false, { 0,0,40,40 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::NEXT);
	trader.button_next = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 40,585 }, { 0,0 }, true,false, { 0,0,40,40 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::PREV);
	trader.Scroll = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { MiddleScreenW + 130, 560 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::SCROLL);
	trader.Scroll_1 = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { MiddleScreenW + 130, 590 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::SCROLL);
	trader.Scroll_2 = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { MiddleScreenW + 130, 620 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::SCROLL);
}

void j1InGameUI::Activate_Menu()
{
	menu.Resume_button->enabled = !menu.Resume_button->enabled;
	menu.Return_button->enabled = !menu.Return_button->enabled;
	menu.Exit_button->enabled = !menu.Exit_button->enabled;
	menu.Load->enabled = !menu.Load->enabled;
	menu.Save->enabled = !menu.Save->enabled;
	menu.Image->enabled = !menu.Image->enabled;
	menu.Scroll->enabled = !menu.Scroll->enabled;
}

void j1InGameUI::Activate_Trading()
{
	trading.Scroll->enabled = true;
	trading.Scroll_1->enabled = true;
	trading.Scroll_2->enabled = true;
	trading.back->enabled = true;
}

void j1InGameUI::Deactivate_Trading()
{
	trading.Scroll->enabled = false;
	trading.Scroll_1->enabled = false;
	trading.Scroll_2->enabled = false;
	trading.back->enabled = false;
}

void j1InGameUI::Activate_Trader()
{
	trader.buton_prev->enabled = true;
	trader.button_next->enabled = true;
	trader.image->enabled = true;
	trader.Scroll->enabled = true;
	trader.Scroll_1->enabled = true;
	trader.Scroll_2->enabled = true;
		
}

void j1InGameUI::Deactivate_Trader()
{
	trader.buton_prev->enabled = false;
	trader.button_next->enabled = false;
	trader.image->enabled = false;
	trader.Scroll->enabled = false;
	trader.Scroll_1->enabled = false;
	trader.Scroll_2->enabled = false;
}

void j1InGameUI::GUI_Event_Manager(GUI_Event type, j1Element* element)
{
 clicking_ui = true;
	switch (type)
	{

	case GUI_Event::EVENT_ONCLICK:
	{

		if (element == menu.Return_button)
			for (std::vector<j1Entity*>::iterator entity = App->entitymanager->entities.begin(); entity != App->entitymanager->entities.end(); entity++)
				if ((*entity)->selected)
				{
					(*entity)->storage.cotton += 10;
					(*entity)->storage.wood += 15;
					(*entity)->storage.metal += 5;
				}

		if (element == menu.Exit_button) {
			App->win->Fullscreen();
		}

		if (element == menu.Save) {
			quit = true;
		}
		if (element == menu.Load) {
			App->scenemanager->ChangeScene(2);
		}

		if (element == menu.Resume_button) {
			Activate_Menu();
		}
		if (element == menu.Menu_button) {
			App->game_pause = !App->game_pause;
			Activate_Menu();
		}
		if (element == building.Boat_Building_Button) {
			selected->BuildUnit(EntityType::BOAT, 1);
		}
		if (element == manager.button_next) {

			if (selected_total != 0)
			{
				offset_modifier = -1;
			}

		}
		if (element == boat.Trade) {
			in_trading = true;
			Deactivate_Boat_Menu();
			Activate_Trading();
			Activate_Trader();
		}
		if (element == building.Trade) {
			in_trading = true;
			Deactivate_Building_Menu();
			Activate_Trading();
			
		}
		if (element == harvester.Trade) {
			in_trading = true;
			Deactivate_Harvester_Menu();
			Activate_Trading();
			
		}
		if (element == trading.back) {
			in_trading = false;
			Deactivate_Trading();
			Deactivate_Trader();
		}
		if (element == manager.buton_prev) {

			if (selected_total != 0)
			{
				offset_modifier = 1;
			}

		}
	}
	}
}

bool j1InGameUI::PostUpdate()
{
	clicking_ui = false;

	bool ret = true;
	if (quit == true) {
		return false;
	}
	return ret;
}

void j1InGameUI::Activate_Building_Menu()
{
	building.Boat_Building_Button->enabled = true;
	building.entity_name_boathouse->enabled = true;
	building.entity_type_Image->enabled = true;
	building.Trade->enabled = true;

}

void j1InGameUI::Deactivate_Building_Menu() 
{
	building.Boat_Building_Button->enabled = false;
	building.entity_name_boathouse->enabled = false;
	building.entity_type_Image->enabled = false;
	building.Trade->enabled = false;
}

void j1InGameUI::Activate_Boat_Menu()
{
	boat.entity_name_boat->enabled = true;
	boat.entity_type_Image->enabled = true;
	boat.Trade->enabled = true;
}

void j1InGameUI::Deactivate_Boat_Menu()
{
	boat.entity_name_boat->enabled = false;
	boat.entity_type_Image->enabled = false;
	boat.Trade->enabled = false;
}

void j1InGameUI::Activate_Harvester_Menu()
{
	harvester.entity_name_Harvester->enabled = true;
	harvester.entity_type_Image->enabled = true;
	harvester.Trade->enabled = true;
}

void j1InGameUI::Deactivate_Harvester_Menu()
{
	harvester.entity_name_Harvester->enabled = false;
	harvester.entity_type_Image->enabled = false;
	harvester.Trade->enabled = false;
}

void j1InGameUI::Manage_Entity_UI(j1Entity* entity)
{
	if (entity != nullptr) {
	
		Activate_Manager();
		switch (entity->type)
		{
		case EntityType::BOATHOUSE:
			if (in_trading != true)
			Activate_Building_Menu();
			Deactivate_Boat_Menu();
			Deactivate_Harvester_Menu();
			break;
		case EntityType::BOAT:
			if(in_trading != true)
			Activate_Boat_Menu();
			Deactivate_Building_Menu();
			Deactivate_Harvester_Menu();
			break;
		case EntityType::HARVESTER:
			if (in_trading != true)
			Activate_Harvester_Menu();
			Deactivate_Building_Menu();
			Deactivate_Boat_Menu();
			break;
		case EntityType::NONE:
			Activate_Building_Menu();
			Deactivate_Boat_Menu();
			Deactivate_Harvester_Menu();
			break;
		}
	}
	else {
		Deactivate_Manager();
		Deactivate_Boat_Menu();
		Deactivate_Building_Menu(); 
		Deactivate_Harvester_Menu();
		Deactivate_Trading();
		Deactivate_Trader();
	}
}

void j1InGameUI::Activate_Manager()
{
	if (selected_total > 1)
	{
		manager.button_next->enabled = true;
		manager.buton_prev->enabled = true;
	}
	else
	{
		selected_offset = 0;
	}
	manager.image->enabled = true;
}

void j1InGameUI::Deactivate_Manager()
{
	manager.button_next->enabled = false;
	manager.buton_prev->enabled = false;
	manager.image->enabled = false;
}

void j1InGameUI::GetSelectedEntity()
{
	std::vector<j1Entity*> selected_list;
	for (std::vector<j1Entity*>::iterator e = App->entitymanager->entities.begin(); e != App->entitymanager->entities.end(); e++)
	{
		if ((*e)->selected && (*e)->team == 1)
		{
				bool found = false;
				for (std::vector<j1Entity*>::iterator s = selected_list.begin(); s != selected_list.end(); s++)
					if (*e == *s)
					{
						found = true;
						break;
					}
				if (!found)
					selected_list.push_back(*e);
		}
		else
			for (std::vector<j1Entity*>::iterator s = selected_list.begin(); s != selected_list.end(); s++)
				if (*e == *s)
				{
					selected_list.erase(s);
					break;
				}
	}
	selected_total = selected_list.size();

	int counter = 0;

	for (std::vector<j1Entity*>::iterator s = selected_list.begin(); s != selected_list.end(); s++)
	{
		if (*s == selected)
		{
			selected_offset = counter;
			break;
		}
		counter++;
	}

	selected_offset += offset_modifier;
	offset_modifier = 0;

	if (selected_offset < 0)
		selected_offset = selected_list.size() - 1;
	if (selected_offset > selected_list.size() - 1)
		selected_offset = 0;

	counter = 0;
	
	while (counter != selected_list.size())
	{
		std::vector<j1Entity*>::iterator s = selected_list.begin();
		s += counter;
		for (; s != selected_list.end(); s++)
		{
			if (s == selected_list.begin() + selected_offset)
			{
				selected = *s;
	
				App->render->AddBlitEvent(0, nullptr, 0, 0, { selected->GetRenderPositionX(), selected->GetRenderPositionY(), 30, 30 }, false, false, 255, 0, 255, 100);
			}
	
			counter++;
		}
	}
}

void j1InGameUI::Deactivate_All_UI()
{
	Deactivate_Manager();
	Deactivate_Boat_Menu();
	Deactivate_Building_Menu();



	menu.Resume_button->enabled = false;
	menu.Return_button->enabled = false;
	menu.Exit_button->enabled = false;
	menu.Load->enabled = false;
	menu.Save->enabled = false;
	menu.Image->enabled = false;
	menu.Scroll->enabled = false;

	basics.Image->enabled = false;
	basics.Resources->enabled = false;
	menu.Menu_button->enabled = false;
}

void j1InGameUI::Activate_Necessary_UI() {
	basics.Image->enabled = true;
	basics.Resources->enabled = true;
	menu.Menu_button->enabled = true;

}