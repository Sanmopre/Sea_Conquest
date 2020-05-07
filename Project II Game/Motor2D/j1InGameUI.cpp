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
#include "j1MainMenuUI.h"
#include "j1TransitionManager.h"
#include "j1QuestManager.h"
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
	//if (App->entitymanager->selected_n > 0)
	selected = nullptr;
	GetSelectedEntity();
	//else
	//	selected = nullptr;
	if (App->scenemanager->In_Main_Menu == false && selected_total != 0) {
		if (selected->trading_entity != nullptr) {
			if (in_trading)
				Activate_Trader();
			Trading_Manager(selected);
			Update_Resources_Trader(selected->trading_entity);
		}
		else {
			Deactivate_Trader();
		}
		Manage_Entity_UI(selected);
		Update_Resources(selected);
		Activate_Resource_Menu();

	}
	else {
		in_trading = false;
		Deactivate_Trader();
		Manage_Entity_UI(nullptr);
		Deactivate_Resource_Menu();
	}


	//UPDATE INFORMATION
	if (selected != nullptr) {
		sprintf_s(information.attack_text, 10, "%7d", 50);
		information.health = selected->max_health;
		sprintf_s(information.health_text, 10, "%7d", information.health);
		sprintf_s(information.max_resource_text, 10, "%7d", selected->load.maxweight);
		sprintf_s(information.speed_text, 10, "%7d", 100);
	}

	//UPDATE RESOURCES
	if (selected != nullptr) {
		sprintf_s(cotton_resource, 10, "%7d", selected->load.cotton);
		sprintf_s(wood_resource, 10, "%7d", selected->load.wood);
		sprintf_s(metal_resource, 10, "%7d", selected->load.metal);
	}

	//COST_UPDATE
	if (in_hover == true) {
		sprintf_s(cotton_t, 10, "%7d", cotton);
		sprintf_s(wood_t, 10, "%7d", wood);
		sprintf_s(metal_t, 10, "%7d", metal);
	}

	//COIN COST UPDATE
	if (in_hover_coin_cost == true) {
		sprintf_s(coin_cost_t, 10, "%7d", coin_cost);
	}
	
	sprintf_s(coins_t, 10, "%7d", coins);

	//TRADING RESOURCES
	if (selected != nullptr) {
		sprintf_s(text_max, 10, "%7d", selected->load.Total());
		sprintf_s(text_type_0, 10, "%7d", selected->load.cotton);
		sprintf_s(text_type_1, 10, "%7d", selected->load.wood);
		sprintf_s(text_type_2, 10, "%7d", selected->load.metal);
		//TRADER RESOURCES
		if (selected->trading_entity != nullptr) {			
			sprintf_s(trader_max_text, 10, "%7d", selected->trading_entity->load.Total());
			sprintf_s(trader_text_type_0, 10, "%7d", selected->trading_entity->load.cotton);
			sprintf_s(trader_text_type_1, 10, "%7d", selected->trading_entity->load.wood);
			sprintf_s(trader_text_type_2, 10, "%7d", selected->trading_entity->load.metal);
		}
	}

	//PRINT RESOURCES
	if (App->scenemanager->In_Main_Menu == false) {

		if (selected_total != 0) {
			App->fonts->BlitText(0, 8, 1, cotton_resource);
			App->fonts->BlitText(120, 8, 1, wood_resource);
			App->fonts->BlitText(245, 8, 1, metal_resource);
		}

		if (in_trading == true) {
			App->fonts->BlitText( 75, 658, 1, text_max);
			App->fonts->BlitText(255, 558, 1, text_type_0);
			App->fonts->BlitText(255, 598, 1, text_type_1);
			App->fonts->BlitText(255, 638, 1, text_type_2);
		}

		if (in_trader == true) {
			App->fonts->BlitText(635, 658, 1, trader_max_text);
			App->fonts->BlitText(810, 558, 1, trader_text_type_0);
			App->fonts->BlitText(810, 598, 1, trader_text_type_1);
			App->fonts->BlitText(810, 638, 1, trader_text_type_2);
		}

		if (in_hover == true) {
			App->fonts->BlitText(75, 485, 1, cotton_t);
			App->fonts->BlitText(170, 485, 1, wood_t);
			App->fonts->BlitText(270, 485, 1, metal_t);
		}

		if (information.in_info == true) {
			App->fonts->BlitText(100, 120, 1,information.attack_text);
			App->fonts->BlitText(100, 150, 1, information.health_text);
			App->fonts->BlitText(100, 180, 1, information.speed_text);
			App->fonts->BlitText(100, 210, 1, information.max_resource_text);
		}

		if (in_hover_coin_cost == true) {
			App->fonts->BlitText(70, 485, 1, coin_cost_t);
		}

		if (in_townhall == true) {
			App->fonts->BlitText(105, 660, 1, coins_t);
		}
	}

	//MENU FROM ESC
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && App->scenemanager->In_Main_Menu == false) {
	//	Activate_Win_Menu();
	//	Activate_Defeat_Menu();
		Activate_Menu();
	}


	//MANAGE COST MENU
	/////////////////////////////////////////////////
	if(in_hover == false)
	Deactivate_Cost_Menu();

	if (in_hover_info == false)
	Deactivate_Information();

	if (in_hover_coin_cost == false)
	Deactivate_Coin_Cost();

	in_hover_coin_cost = false;
	in_hover_info = false;
	in_hover = false;
	////////////////////////////////////////////////////




	//MANAGE GODMODE UI
	if (App->scenemanager->In_Main_Menu == false) {
		if(App->godmode == true){
			godmode.Godmode_Label->enabled = true;
			godmode.Image->enabled = true;
		}
		else {
		godmode.Godmode_Label->enabled = false;
			godmode.Image->enabled = false;
		}
	}
	else {
		godmode.Godmode_Label->enabled = false;
		godmode.Image->enabled = false;
	}
	


	//MANAGE THE INFO BUTTTON 
	//TEMPORAL FIX
	///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
	if (in_trading == true)
		manager.info->enabled = false;
	///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////

	return true;
}

bool j1InGameUI::CleanUp()
{
	selected = nullptr;
	return true;

}

//UI FUNCTIONS
void j1InGameUI::Add_UI()
{
	//MENU
	menu.Menu_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, {width -50,10 }, { 0,0 }, true, true, { 0,0,40,40 }, "", this,false, false, SCROLL_TYPE::SCROLL_NONE,true, TEXTURE::OPTIONS);
	menu.Return_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH-140 }, { 40,30 }, true, false, { 0,0,200,65 }, "ADD RESOURCES", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);
	menu.Resume_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH -60}, { 60,30 }, true, false, { 0,0,200,65 }, "RESUME", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);
	menu.Exit_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH + 15 }, {60,30 }, true, false, { 0,0,200,65 }, "FULLSCREEN", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);
	menu.Save = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH +90 }, { 70,30 }, true, false, { 0,0,200,65 }, "QUIT", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);
	menu.Load = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 25,MiddleScreenH +165}, { 50,30 }, true, false, { 0,0,200,65 }, "MAIN MENU", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);
	menu.Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { MiddleScreenW - 50,0 }, { 0,0 }, true, false, { 0, 0,350,500 },"", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::IMAGE);


	//ENTITY_MANAGER_UI
	manager.image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { MiddleScreenW - 470,525 }, { 0,0 }, true, false, { 0, 0,350,170 }, "", this,false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::MANAGER_IMAGE);
	manager.button_next = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW - 510,585 }, { 0,0 }, true, true, { 0,0,40,40 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::PREV);
	manager.buton_prev = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW -120,585 }, { 0,0 }, true, true, { 0,0,40,40 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::NEXT);
	manager.info = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW - 180,545 }, { 0,0 }, true, true, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::INFO);
	
	//BOAT_MENU
	boat.entity_name_boat = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 150,555 }, { 0,0 }, true, true, { 0,0,40,40 }, "BOAT ", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	boat.entity_type_Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 110,550 }, { 0,0 }, true, false, { 0, 0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BOAT_IMAGE);
	boat.Trade = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 120,655 }, { 0,0 }, true, true, { 0,0,30,30 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADE);


	//HARVESTER
	harvester.entity_name_Harvester = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 150,555 }, { 0,0 }, true, true, { 0,0,40,40 }, "HARVESTER", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	harvester.entity_type_Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 110,550 }, { 0,0 }, true, false, { 0, 0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::HARVESTER);
	harvester.Trade = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 120,655 }, { 0,0 }, true, true, { 0,0,30,30 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADE);
	harvester.boathouse = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 110,600 }, { 0,0 }, true, true, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUILDING_IMAGE);
	harvester.Storage = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 145,600 }, { 0,0 }, true, true, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::STORAGE);
	harvester.Automatic = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW - 180,655 }, { 0,0 }, true, true, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::AUTOMATIC);

	//SHIP
	ship.entity_name = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 150,555 }, { 0,0 }, true, false, { 0,0,40,40 }, "BATTLE SHIP", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	ship.entity_type_Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 110,550 }, { 0,0 }, true, false, { 0, 0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::SHIP);
	ship.Trade = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 120,655 }, { 0,0 }, true, false, { 0,0,30,30 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADE);

	//BALLOON
	balloon.entity_name = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 150,555 }, { 0,0 }, true, false, { 0,0,40,40 }, "BATTLE BALLOON", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	balloon.entity_type_Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 110,550 }, { 0,0 }, true, false, { 0, 0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BALLOON);
	balloon.Trade = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 120,655 }, { 0,0 }, true, false, { 0,0,30,30 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADE);

	//TOWNHALL
	townhall.entity_name_townhall = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 150,555 }, { 0,0 }, true, true, { 0,0,40,40 }, "TOWNHALL", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	townhall.entity_type_Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 110,550 }, { 0,0 }, true, false, { 0, 0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TOWNHALL);
	townhall.coins_image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 120,655 }, { 0,0 }, true, false, { 0, 0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::COIN);
	townhall.lvl_up = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 110,600 }, { 0,0 }, true, false, { 0, 0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::LVLUP);
	townhall.Quest_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 150,600 }, { 0,0 }, true, false, { 0, 0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::QUEST);
	
	
	//BOAT_BUILDER_MENU
	building.Boat_Building_Button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 110,600 }, { 0,0 }, true, true, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BOAT_IMAGE);
	building.entity_name_boathouse = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 150,555 }, { 0,0 }, true, true, { 0,0,40,40 }, "BOAT HOUSE", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	building.entity_type_Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 110,550 }, { 0,0 }, true, false, { 0, 0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUILDING_IMAGE);
	building.Trade = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 120,655 }, { 0,0 }, true, true, { 0,0,30,30 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADE);

	//STORAGE_MENU
	storage.Harvester_builder_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 110,600 }, { 0,0 }, true, true, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::HARVESTER);
	storage.entity_name_Storage = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 150,555 }, { 0,0 }, true, false, { 0,0,40,40 }, "STORAGE", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	storage.entity_type_Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 110,550 }, { 0,0 }, true, false, { 0, 0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::STORAGE);
	storage.Trade = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 120,655 }, { 0,0 }, true, false, { 0,0,30,30 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADE);

	//QUEST SELECTOR
	quest_selector.Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { MiddleScreenW + 80,525 }, { 0,0 }, true, false, { 0, 0,350,170 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::MANAGER_IMAGE);
	quest_selector.Quest_1 = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 110,550 }, { 50,5 }, true, false, { 0,0,30,30 },"DESTROY 15 ENEMY BOATS", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::QUEST);
	quest_selector.Quest_2 = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 110,600 }, { 50,5 }, true, false, { 0,0,30,30 }, "DESTROY 1 ENEMY STRUCTURE", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::QUEST);
	quest_selector.Quest_3 = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 110,650 }, { 50,5 }, true, false, { 0,0,30,30 }, "BUILD 10 BOATS", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::QUEST);

	//UI BASICS ALWAYS ACTIVE
	basics.Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, {0,520 }, { 0,0 }, true, true, { 0, 0,1280,200 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::MAIN_IMAGE);
	
	//RESOURCES 
	resources.Resources = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, {0,0 }, { 0,0 }, true, true, { 0, 0,400,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::RESOURCES_IMAGE);


	//TRADING_MENU
	trading.Scroll = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { 110, 550 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_COTTON, true, TEXTURE::SCROLL);
	trading.Scroll_1 = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { 110, 590 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_WOOD, true, TEXTURE::SCROLL);
	trading.Scroll_2 = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { 110, 630 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_METAL, true, TEXTURE::SCROLL);
	trading.back = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 120,655 }, { 0,0 }, true, true, { 0,0,30,30 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADE);

	//TRADER 
	trader.image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { MiddleScreenW + 80,525 }, { 0,0 }, true, false, { 0, 0,350,170 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::MANAGER_IMAGE);
	trader.buton_prev = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 430 ,585 }, { 0,0 }, true, false, { 0,0,40,40 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::NEXT);
	trader.button_next = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 40,585 }, { 0,0 }, true,false, { 0,0,40,40 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::PREV);
	trader.Scroll = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { MiddleScreenW + 130, 550 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_COTTON, true, TEXTURE::SCROLL);
	trader.Scroll_1 = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { MiddleScreenW + 130,  590 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_WOOD, true, TEXTURE::SCROLL);
	trader.Scroll_2 = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { MiddleScreenW + 130, 630 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_METAL, true, TEXTURE::SCROLL);

	trader.button_trade_1 = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 480 ,545 }, { 0,0 }, true, false, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADER_IZQUIERDA);
	trader.button_trade_2 = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 480,545 + 35 }, { 0,0 }, true, false, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADER_IZQUIERDA);
	trader.button_trade_3 = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 480 ,545 + 70 }, { 0,0 }, true, false, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADER_IZQUIERDA);
	trader.button_trade_4 = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 480 + 32,545 }, { 0,0 }, true, false, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADER_DERECHA);
	trader.button_trade_5 = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 480 + 32,545 + 35}, { 0,0 }, true, false, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADER_DERECHA);
	trader.button_trade_6 = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 480 + 32,545 + 70}, { 0,0 }, true, false, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADER_DERECHA);
		
	//WIN
	win.Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, {600 - 150,225  }, { 0,0 }, true, false, { 0, 0,350,170 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::MANAGER_IMAGE);
	win.Label = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, {750 - 150, 280 }, { 0,0 }, true, false, { 0,0,40,40 }, "VICTORY!!", this, false, false, SCROLL_TYPE::SCROLL_NONE,true);
	win.Back_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, {680 - 150,315 }, { 65,30 }, true, false, { 0,0,200,65 }, "MAIN MENU", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);

	//DEFEAT
	defeat.Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 600 - 150,225 }, { 0,0 }, true, false, { 0, 0,350,170 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::MANAGER_IMAGE);
	defeat.Label = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 750 - 150, 280 }, { 0,0 }, true, false, { 0,0,40,40 }, "DEFEAT", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	defeat.Back_button = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 680 - 150,315 }, { 65,30 }, true, false, { 0,0,200,65 }, "MAIN MENU", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::BUTON);

	//COST
	cost.Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 95,475 }, { 0,0 }, true, true, { 0, 0,300,40 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::COST);

	//GODMODE
	godmode.Godmode_Label = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 25, 78 }, { 0,0 }, true, true, { 0,0,40,40 }, "GOD MODE ACTIVATED!", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	godmode.Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 10,63 }, { 0,0 }, true, false, { 0, 0,145,40 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::GODMODE);

	//INFORMATION
	information.Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, {20,50 }, { 0,0 }, true, false, { 0, 0,200,200 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::INFO_IMAGE);
	information.Text = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 90, 70 }, { 0,0 }, true, true, { 0,0,40,40 }, "UNIT STATS", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	information.Attack = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 30, 120 }, { 0,0 }, true, true, { 0,0,40,40 }, "ATTACK", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	information.Health = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 30, 150 }, { 0,0 }, true, true, { 0,0,40,40 }, "HEALTH", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	information.Speed = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 30, 180 }, { 0,0 }, true, true, { 0,0,40,40 }, "SPEED", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	information.Max_resource= App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 30, 210 }, { 0,0 }, true, true, { 0,0,40,40 }, "MAX RESOURCE", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);

	//COINCOST
	coincost.Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 95,475 }, { 0,0 }, true, true, { 0, 0,100,40 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::COIN_COST);
}

void j1InGameUI::Activate_Menu()
{
	menu.Resume_button->enabled = !menu.Resume_button->enabled;
	menu.Return_button->enabled = !menu.Return_button->enabled;
	menu.Exit_button->enabled = !menu.Exit_button->enabled;
	menu.Load->enabled = !menu.Load->enabled;
	menu.Save->enabled = !menu.Save->enabled;
	menu.Image->enabled = !menu.Image->enabled;
	App->game_pause = !App->game_pause;
}

void j1InGameUI::Activate_Cost_Menu()
{
	cost.Image->enabled = true;
	in_hover = true;
}

void j1InGameUI::Deactivate_Cost_Menu()
{
	cost.Image->enabled = false;
	in_hover = false;
}

void j1InGameUI::Activate_Coin_Cost()
{
	in_hover_coin_cost = true;
	coincost.Image->enabled = true;
}

void j1InGameUI::Deactivate_Coin_Cost()
{
	in_hover_coin_cost = false;
	coincost.Image->enabled = false;
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
	trader.button_trade_1->enabled = true;
	trader.button_trade_2->enabled = true;
	trader.button_trade_3->enabled = true;
	trader.button_trade_4->enabled = true;
	trader.button_trade_5->enabled = true;
	trader.button_trade_6->enabled = true;
	in_trader = true;

}

void j1InGameUI::Deactivate_Trader()
{
	trader.buton_prev->enabled = false;
	trader.button_next->enabled = false;
	trader.image->enabled = false;
	trader.Scroll->enabled = false;
	trader.Scroll_1->enabled = false;
	trader.Scroll_2->enabled = false;
	trader.button_trade_1->enabled = false;
	trader.button_trade_2->enabled = false;
	trader.button_trade_3->enabled = false;
	trader.button_trade_4->enabled = false;
	trader.button_trade_5->enabled = false;
	trader.button_trade_6->enabled = false;
	in_trader = false;
}

void j1InGameUI::GUI_Event_Manager(GUI_Event type, j1Element* element)
{
 clicking_ui = true;
	switch (type)
	{

	case GUI_Event::EVENT_ONCLICK:
	{
		App->audio->PlayFx(App->audio->ui_wood_hit);
		if (element == menu.Return_button) {

			for (std::vector<j1Entity*>::iterator entity = App->entitymanager->entities.begin(); entity != App->entitymanager->entities.end(); entity++)
				if ((*entity)->selected)
				{
					if ((*entity)->load.maxweight > (*entity)->load.Weight()) {
						(*entity)->load.cotton += 10;
						(*entity)->load.wood += 10;
						(*entity)->load.metal += 10;
					}
				}
		}
		if (element == menu.Exit_button) {
			App->win->Fullscreen();
		}

		if (element == quest_selector.Quest_1)
			App->quest->current_quest = App->quest->Set_Quest(QUEST::KILL_15_BOATS);


		if (element == quest_selector.Quest_2)
			App->quest->current_quest = App->quest->Set_Quest(QUEST::DESTROY_ENEMY_STRUCTURE);

		if (element == quest_selector.Quest_3)
			App->quest->current_quest = App->quest->Set_Quest(QUEST::BUILD_10_BOATS);


		if (element == townhall.Quest_button) {
			if (in_quest_manager == false)
				Activate_Quest_Selector();
			else
				Deactivate_Quest_Selector();
		}

		if (element == menu.Save) {
			quit = true;
		}
		if (element == menu.Load) {
			App->transitions->LinesAppearing(Black, 0.75f, 2);
		}

		if (element == win.Back_button) {
			App->transitions->LinesAppearing(Black, 0.75f, 2);
		}

		if (element == defeat.Back_button) {
			App->transitions->LinesAppearing(Black, 0.75f, 2);
		}

		if (element == menu.Resume_button) {
			Activate_Menu();
		}
		if (element == menu.Menu_button) {
			
			Activate_Menu();
		}
		if (element == building.Boat_Building_Button) {
			if (Cost_Function(selected, 20, 40, 0))
			selected->BuildUnit(EntityType::BOAT, 1);
		}

		if (element == storage.Harvester_builder_button) {
			if(Cost_Function(selected, 0, 0, 30))
			selected->BuildUnit(EntityType::HARVESTER, 1);
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

		}
		if (element == building.Trade) {
			in_trading = true;
			Deactivate_Building_Menu();
			Activate_Trading();
			
		}
		if (element == ship.Trade) {
			in_trading = true;
			Deactivate_Ship();
			Activate_Trading();
		}
		if (element == balloon.Trade) {
			in_trading = true;
			Deactivate_Balloon();
			Activate_Trading();
		}
		if (element == harvester.Trade) {
			in_trading = true;
			Deactivate_Harvester_Menu();
			Activate_Trading();
		}

		if (element == storage.Trade) {
			in_trading = true;
			Deactivate_Storage_Menu();
			Activate_Trading();

		}

		if (element == trading.back) {
			in_trading = false;
			Deactivate_Trading();
			Deactivate_Trader();
		}
		if (element == trader.button_next) {
			selected->trading_offset_modifier++;
		}
		if (element == trader.buton_prev) {
			selected->trading_offset_modifier--;
		}
		if (element == manager.buton_prev) {

			if (selected_total != 0)
			{
				offset_modifier = 1;
			}

		}

		if (element == harvester.boathouse) {
			if (Cost_Function(selected, 0, 40, 40))
				selected->BuildStructure(EntityType::BOATHOUSE);
		}
		if (element == harvester.Storage) {
			if (Cost_Function(selected, 0, 50, 10))
				selected->BuildStructure(EntityType::STORAGE);
		}
		if (element == harvester.Automatic) {
			selected->SetAutomatic();
		}

		if (element == trader.button_trade_1) {
			selected->load.Transfer(Material::COTTON, &selected->trading_entity->load.cotton, 5 );
		}
		if (element == trader.button_trade_2) {
			selected->load.Transfer(Material::WOOD, &selected->trading_entity->load.wood, 5);
		}
		if (element == trader.button_trade_3) {
			selected->load.Transfer(Material::METAL, &selected->trading_entity->load.metal, 5);
		}
		if (element == trader.button_trade_4) {
			selected->trading_entity->load.Transfer(Material::COTTON, &selected->load.cotton, 5);
		}
		if (element == trader.button_trade_5) {
			selected->trading_entity->load.Transfer(Material::WOOD, &selected->load.wood, 5);
		}
		if (element == trader.button_trade_6) {
			selected->trading_entity->load.Transfer(Material::METAL, &selected->load.metal, 5);
		}
	}
	break;
	case GUI_Event::EVENT_LEFTCLICK:
		if (element == trader.button_trade_1) {
			selected->load.Transfer(Material::COTTON, &selected->trading_entity->load.cotton, selected->trading_entity->load.cotton);
		}
		if (element == trader.button_trade_2) {
			selected->load.Transfer(Material::WOOD, &selected->trading_entity->load.wood, selected->trading_entity->load.wood);
		}
		if (element == trader.button_trade_3) {
			selected->load.Transfer(Material::METAL, &selected->trading_entity->load.metal, selected->trading_entity->load.metal);
		}
		if (element == trader.button_trade_4) {
			selected->trading_entity->load.Transfer(Material::COTTON, &selected->load.cotton, selected->load.cotton);
		}
		if (element == trader.button_trade_5) {
			selected->trading_entity->load.Transfer(Material::WOOD, &selected->load.wood, selected->load.wood);
		}
		if (element == trader.button_trade_6) {
			selected->trading_entity->load.Transfer(Material::METAL, &selected->load.metal, selected->load.metal);
		}
		break;
	case GUI_Event::EVENT_HOVER:
		if (element == building.Boat_Building_Button) {
			Activate_Cost_Menu();
			cotton = 20;
			wood = 40;
			metal = 0;
		}
		if (element == harvester.boathouse) {
			Activate_Cost_Menu();
			cotton = 0;
			wood = 40;
			metal = 40;
		}
		if (element == harvester.Storage) {
			Activate_Cost_Menu();
			cotton = 0;
			wood = 50;
			metal = 10;
		}

		if (element == storage.Harvester_builder_button) {
			Activate_Cost_Menu();
			cotton = 0;
			wood = 0;
			metal = 30;
		}
		if (element == manager.info) {
			Activate_Information();
		}
		
		if (element == townhall.lvl_up) {
			Activate_Coin_Cost();
			coin_cost = 10;
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

void j1InGameUI::Activate_Resource_Menu()
{
	resources.Resources->enabled = true;
}

void j1InGameUI::Deactivate_Resource_Menu()
{
	resources.Resources->enabled = false;
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
	harvester.boathouse->enabled = true;
	harvester.Storage->enabled = true;
	harvester.Automatic->enabled = true;
}

void j1InGameUI::Deactivate_Harvester_Menu()
{
	harvester.entity_name_Harvester->enabled = false;
	harvester.entity_type_Image->enabled = false;
	harvester.Trade->enabled = false;
	harvester.boathouse->enabled = false;
	harvester.Storage->enabled = false;
	harvester.Automatic->enabled = false;
}

void j1InGameUI::Activate_Townhall_Menu()
{
	townhall.entity_name_townhall->enabled = true;
	townhall.entity_type_Image->enabled = true;
	townhall.coins_image->enabled = true;
	townhall.lvl_up->enabled = true;
	townhall.Quest_button->enabled = true;
	in_townhall = true;
}

void j1InGameUI::Deactivate_Townhall_Menu()
{
	townhall.entity_name_townhall->enabled = false;
	townhall.entity_type_Image->enabled = false;
	townhall.coins_image->enabled = false;
	townhall.lvl_up->enabled = false;
	townhall.Quest_button->enabled = false;
	in_townhall = false;
}

void j1InGameUI::Activate_Storage_Menu()
{
	storage.entity_name_Storage->enabled = true;
	storage.entity_type_Image->enabled = true;
	storage.Trade->enabled = true;
	storage.Harvester_builder_button->enabled = true;
}

void j1InGameUI::Deactivate_Storage_Menu()
{
	storage.entity_name_Storage->enabled = false;
	storage.entity_type_Image->enabled = false;
	storage.Trade->enabled = false;
	storage.Harvester_builder_button->enabled = false;
}

void j1InGameUI::Activate_Win_Menu()
{
	win.Label->enabled = true;
	win.Back_button->enabled = true;
	win.Image->enabled = true;
}

void j1InGameUI::Deactivate_Win_Menu()
{
	if (win.Label != nullptr)
	{
		win.Label->enabled = false;
			win.Back_button->enabled = false;
			win.Image->enabled = false;
	}
}

void j1InGameUI::Activate_Defeat_Menu()
{
	defeat.Back_button->enabled = true;
	defeat.Image->enabled = true;
	defeat.Label->enabled = true;

}

void j1InGameUI::Deactivate_Defeat_Menu()
{ 
	defeat.Back_button->enabled = false;
	defeat.Image->enabled = false;
	defeat.Label->enabled = false;
}

void j1InGameUI::Activate_Information()
{
	information.Image->enabled = true;
	information.Text->enabled = true;
	information.Health->enabled = true;
	information.Attack->enabled = true;
	information.Speed->enabled = true;
	information.Max_resource->enabled = true;
	information.in_info = true;
	in_hover_info = true;
}

void j1InGameUI::Deactivate_Information()
{
	information.Image->enabled = false;
	information.Text->enabled = false;
	information.Health->enabled = false;
	information.Attack->enabled = false;
	information.Speed->enabled = false;
	information.Max_resource->enabled = false;
	information.in_info = false;
	in_hover_info = false;
}

void j1InGameUI::Activate_Balloon()
{
	balloon.entity_name->enabled = true;
	balloon.entity_type_Image->enabled = true;
	balloon.Trade->enabled = true;
}

void j1InGameUI::Deactivate_Balloon()
{
	balloon.entity_name->enabled = false;
	balloon.entity_type_Image->enabled = false;
	balloon.Trade->enabled = false;
}

void j1InGameUI::Activate_Ship()
{
	ship.entity_name->enabled = true;
	ship.entity_type_Image->enabled = true;
	ship.Trade->enabled = true;
}

void j1InGameUI::Deactivate_Ship()
{
	ship.entity_name->enabled = false;
	ship.entity_type_Image->enabled = false;
	ship.Trade->enabled = false;
}

void j1InGameUI::Activate_Quest_Selector()
{
	quest_selector.Image->enabled = true;
	quest_selector.Quest_1->enabled = true;
	quest_selector.Quest_2->enabled = true;
	quest_selector.Quest_3->enabled = true;
	in_quest_manager = true;
}

void j1InGameUI::Deactivate_Quest_Selector()
{
	quest_selector.Image->enabled = false;
	quest_selector.Quest_1->enabled = false;
	quest_selector.Quest_2->enabled = false;
	quest_selector.Quest_3->enabled = false;
	in_quest_manager = false;
}

void j1InGameUI::Update_Bar(j1Element* scroll, float resource, float total_resource, Material material)
{
	float percentage = 0;
	percentage = resource / total_resource;

	scroll->Button->inside_position.x = -(235 * percentage);
	switch (material) {
	case Material::COTTON:
		scroll->Button->map_position.x = 110 + (235 * percentage);
		break;
	case Material::WOOD:
		scroll->Button->map_position.x = 110 + (235 * percentage) * 4;
		break;
	case Material::METAL:
		scroll->Button->map_position.x = 110 + (235 * percentage) * 10;
		break;
	}
}

void j1InGameUI::Update_Bar_Trader(j1Element* scroll, float resource, float total_resource, Material material)
{
	float percentage = 0;
	percentage = resource / total_resource;

	scroll->Button->inside_position.x = -(235 * percentage);
	switch (material) {
	case Material::COTTON:
		scroll->Button->map_position.x = MiddleScreenW + 130 + (235 * percentage);
		break;
	case Material::WOOD:
		scroll->Button->map_position.x = MiddleScreenW + 130 + (235 * percentage) * 4;
		break;
	case Material::METAL:
		scroll->Button->map_position.x = MiddleScreenW + 130 + (235 * percentage) * 10;
		break;
	}
}

void j1InGameUI::Update_Resources(j1Entity* entity)
{
		Update_Bar(trading.Scroll, entity->load.cotton, entity->load.maxweight, Material::COTTON);
		Update_Bar(trading.Scroll_1, entity->load.wood, entity->load.maxweight, Material::WOOD);
		Update_Bar(trading.Scroll_2, entity->load.metal, entity->load.maxweight, Material::METAL);
}

void j1InGameUI::Update_Resources_Trader(j1Entity* entity)
{
		Update_Bar_Trader(trader.Scroll, entity->load.cotton, entity->load.maxweight, Material::COTTON);
		Update_Bar_Trader(trader.Scroll_1, entity->load.wood, entity->load.maxweight, Material::WOOD);
		Update_Bar_Trader(trader.Scroll_2, entity->load.metal, entity->load.maxweight, Material::METAL);
}

void j1InGameUI::Trading_Manager(j1Entity* entity)
{
		//selected->load.Transfer(Material::COTTON, &selected->trading_entity->load.cotton,10);
}

bool j1InGameUI::Cost_Function(j1Entity* entity, int cotton, int wood, int metal)
{
	if (entity->load.cotton >= cotton && entity->load.wood >= wood && entity->load.metal >= metal) 
	{
		entity->load.cotton = entity->load.cotton - cotton;
		entity->load.wood = entity->load.wood - wood;
		entity->load.metal = entity->load.metal - metal;
		return true;
	}
	else
	return false;
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
			Deactivate_Storage_Menu();
			Deactivate_Townhall_Menu();
			Deactivate_Quest_Selector();
			break;

		case EntityType::BOAT:
			if(in_trading != true)
			Activate_Boat_Menu();
			Deactivate_Building_Menu();
			Deactivate_Harvester_Menu();
			Deactivate_Storage_Menu();
			Deactivate_Townhall_Menu();
			Deactivate_Quest_Selector();
			break;

		case EntityType::HARVESTER:
			if (in_trading != true)
			Activate_Harvester_Menu();
			Deactivate_Building_Menu();
			Deactivate_Boat_Menu();
			Deactivate_Storage_Menu();
			Deactivate_Townhall_Menu();
			Deactivate_Quest_Selector();
			break;

		case EntityType::STORAGE:
			if (in_trading != true)
			Activate_Storage_Menu();
			Deactivate_Townhall_Menu();
			Deactivate_Building_Menu();
			Deactivate_Boat_Menu();
			Deactivate_Harvester_Menu();	
			Deactivate_Quest_Selector();
			break;

		case EntityType::TOWNHALL:
			if (in_trading != true)
			Activate_Townhall_Menu();
			Deactivate_Storage_Menu();
			Deactivate_Building_Menu();
			Deactivate_Boat_Menu();
			Deactivate_Harvester_Menu();
			break;
		
		case EntityType::NONE:
			Deactivate_Building_Menu();
			Deactivate_Boat_Menu();
			Deactivate_Harvester_Menu();
			Deactivate_Storage_Menu();
			Deactivate_Townhall_Menu();
			Deactivate_Quest_Selector();
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
		Deactivate_Townhall_Menu();
		Deactivate_Storage_Menu();
		Deactivate_Quest_Selector();
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
	manager.info->enabled = true;
}

void j1InGameUI::Deactivate_Manager()
{
	manager.button_next->enabled = false;
	manager.buton_prev->enabled = false;
	manager.image->enabled = false;
	manager.info->enabled = false;
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
	
				App->render->AddBlitEvent(0, nullptr, 0, 0, selected->selectable_area, false, false, 255, 0, 255, 100);
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
	Deactivate_Defeat_Menu();
	Deactivate_Win_Menu();
	Deactivate_Resource_Menu();
	Deactivate_Quest_Selector();
	App->quest->Close_Quest_Manager();

	menu.Resume_button->enabled = false;
	menu.Return_button->enabled = false;
	menu.Exit_button->enabled = false;
	menu.Load->enabled = false;
	menu.Save->enabled = false;
	menu.Image->enabled = false;

	basics.Image->enabled = false;
	menu.Menu_button->enabled = false;
}

void j1InGameUI::Activate_Necessary_UI() {
	basics.Image->enabled = true;
	menu.Menu_button->enabled = true;
	App->quest->Open_Quest_Manager();
}