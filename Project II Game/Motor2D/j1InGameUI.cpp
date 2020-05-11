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
#include "j1Explanation.h"
#include "j1Font.h"
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
	selected_last_frame = selected;
	selected = nullptr;
	GetSelectedEntity();

	if (App->scenemanager->In_Main_Menu == false && selected_total != 0) {
		if (selected->trading_entity != nullptr) {

			if (in_trading)
			Activate_Trader();

			Update_Resources_Trader(selected->trading_entity);
		}
		else {
			Deactivate_Trader();
		}

		Manage_Entity_UI(selected);
		Update_Resources(selected);
		Activate_Resource_Menu();

		if (selected->type != EntityType::TOWNHALL) {
			Deactivate_Quest_Selector();
		}

	}
	else {
		in_trading = false;
		Deactivate_Trader();
		Manage_Entity_UI(nullptr);
		Deactivate_Resource_Menu();
	}

	if(selected_total == 0)
		Deactivate_Quest_Selector();


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


			Change_Image_Label_Trader(selected->trading_entity);
			if (in_trading) {
				Trader_image->enabled = true;
				Trader_label->enabled = true;
			}
			else {
				Trader_image->enabled = false;
				Trader_label->enabled = false;
			}
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
			App->fonts->BlitText(265, 560, 1, text_type_0);
			App->fonts->BlitText(265, 590, 1, text_type_1);
			App->fonts->BlitText(265, 620, 1, text_type_2);
		}

		if (in_trader == true) {
			App->fonts->BlitText(635, 658, 1, trader_max_text);
			App->fonts->BlitText(810, 560, 1, trader_text_type_0);
			App->fonts->BlitText(810, 590, 1, trader_text_type_1);
			App->fonts->BlitText(810, 620, 1, trader_text_type_2);
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


	if (in_trading == false) {
	
		trading.back->enabled = false;
		trading.Scroll->enabled = false;
		trading.Scroll_1->enabled = false;
		trading.Scroll_2->enabled = false;
	}

	if (!in_trader) {
		Trader_image->enabled = false;
		Trader_label->enabled = false;
	}

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
	

	//ENTITY UI
	entity_ui.image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 90,545 }, { 0,0 }, true, false, { 0, 0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::NONE);
	entity_ui.name = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 130,550 }, { 0,0 }, true, true, { 0,0,40,40 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	entity_ui.trade = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 90,655 }, { 0,0 }, true, true, { 0,0,30,30 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADE);
	entity_ui.button_1 = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 110,600 }, { 0,0 }, true, true, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::NONE);
	entity_ui.button_2 = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 145,600 }, { 0,0 }, true, true, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::NONE);
	entity_ui.button_3 = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 180,600 }, { 0,0 }, true, true, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::NONE);
	entity_ui.button_4 = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 215,600 }, { 0,0 }, true, true, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::NONE);
	entity_ui.button_5 = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW - 180,655 }, { 0,0 }, true, true, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::NONE);
	
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
	trading.Scroll = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { 126, 580 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_COTTON, true, TEXTURE::SCROLL);
	trading.Scroll_1 = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { 126, 610 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_WOOD, true, TEXTURE::SCROLL);
	trading.Scroll_2 = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { 126, 640 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_METAL, true, TEXTURE::SCROLL);
	trading.back = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 90,655 }, { 0,0 }, true, true, { 0,0,30,30 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADE);

	//TRADER 
	trader.image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { MiddleScreenW + 80,525 }, { 0,0 }, true, false, { 0, 0,350,170 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::MANAGER_IMAGE);
	trader.buton_prev = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 430 ,585 }, { 0,0 }, true, false, { 0,0,40,40 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::NEXT);
	trader.button_next = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { MiddleScreenW + 40,585 }, { 0,0 }, true,false, { 0,0,40,40 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::PREV);
	trader.Scroll = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { MiddleScreenW + 130,  580 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_COTTON, true, TEXTURE::SCROLL);
	trader.Scroll_1 = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { MiddleScreenW + 130, 610 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_WOOD, true, TEXTURE::SCROLL);
	trader.Scroll_2 = App->gui->AddElement(GUItype::GUI_SCROLLBAR, nullptr, { MiddleScreenW + 130, 640 }, { 0,0 }, false, false, { 0, 0, 260, 7 }, nullptr, this, false, false, SCROLL_TYPE::SCROLL_METAL, true, TEXTURE::SCROLL);

	trader.button_trade_1 = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 480 ,545 }, { 0,0 }, true, false, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADER_IZQUIERDA);
	trader.button_trade_2 = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 480,545 + 35 }, { 0,0 }, true, false, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADER_IZQUIERDA);
	trader.button_trade_3 = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 480 ,545 + 70 }, { 0,0 }, true, false, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADER_IZQUIERDA);
	trader.button_trade_4 = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 480 + 32,545 }, { 0,0 }, true, false, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADER_DERECHA);
	trader.button_trade_5 = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 480 + 32,545 + 35}, { 0,0 }, true, false, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADER_DERECHA);
	trader.button_trade_6 = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 480 + 32,545 + 70}, { 0,0 }, true, false, { 0,0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::TRADER_DERECHA);
		

	//TRADER IMAGE AND LABEL
	Trader_label = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 680,550 }, { 0,0 }, true, true, { 0,0,40,40 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	Trader_image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 640,545 }, { 0,0 }, true, false, { 0, 0,30,30 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::NONE);

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


		if (element == entity_ui.button_2 && selected->type == EntityType::TOWNHALL) {
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
		if (element == entity_ui.button_1 && selected->type == EntityType::BOATHOUSE) {
			if (Cost_Function(selected, 20, 40, 0))
			selected->BuildUnit(EntityType::BOAT, 1);
		}

		if (element == entity_ui.button_2 && selected->type == EntityType::BOATHOUSE) {
			if (Cost_Function(selected, 10, 10, 60))
				selected->BuildUnit(EntityType::CARRIER, 1);
		}

		if (element == entity_ui.button_3 && selected->type == EntityType::BOATHOUSE) {
			if (Cost_Function(selected, 60, 40, 10))
				selected->BuildUnit(EntityType::BALLOON, 1);
		}

		if (element == entity_ui.button_1 && selected->type == EntityType::STORAGE) {
			if(Cost_Function(selected, 0, 0, 30))
			selected->BuildUnit(EntityType::HARVESTER, 1);
		}

		if (element == manager.button_next) {

			if (selected_total != 0)
			{
				offset_modifier = -1;
			}

		}


		if (element == entity_ui.trade) {
			in_trading = true;
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

		if (element == entity_ui.button_1 && selected->type == EntityType::HARVESTER) {
			if (Cost_Function(selected, 0, 40, 40))
				selected->BuildStructure(EntityType::BOATHOUSE);
		}
		if (element == entity_ui.button_2 && selected->type == EntityType::HARVESTER) {
			if (Cost_Function(selected, 0, 50, 10))
				selected->BuildStructure(EntityType::STORAGE);
		}
		if (element == entity_ui.button_5 && selected->type == EntityType::HARVESTER) {
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
		if (element == entity_ui.button_1 && selected->type == EntityType::BOATHOUSE) {
			Activate_Cost_Menu();
			cotton = 20;
			wood = 40;
			metal = 0;
			if(App->expl->information_mode)
			App->expl->Show_Label(Text::BOAT);
		}
		if (element == entity_ui.button_2 && selected->type == EntityType::BOATHOUSE) {
			Activate_Cost_Menu();
			cotton = 10;
			wood = 10;
			metal = 60;
		}
		if (element == entity_ui.button_3 && selected->type == EntityType::BOATHOUSE) {
			Activate_Cost_Menu();
			cotton = 60;
			wood = 40;
			metal = 20;
		}
		if (element == entity_ui.button_1 && selected->type == EntityType::HARVESTER) {
			Activate_Cost_Menu();
			cotton = 0;
			wood = 40;
			metal = 40;
			if (App->expl->information_mode)
			App->expl->Show_Label(Text::BOATHOUSE);
		}
		if (element == entity_ui.button_2 && selected->type == EntityType::HARVESTER) {
			Activate_Cost_Menu();
			cotton = 0;
			wood = 50;
			metal = 10;
			if (App->expl->information_mode)
			App->expl->Show_Label(Text::STORAGE);
		}

		if (element == entity_ui.button_1 && selected->type == EntityType::STORAGE) {
			Activate_Cost_Menu();
			cotton = 0;
			wood = 0;
			metal = 30;
			if (App->expl->information_mode)
				App->expl->Show_Label(Text::HARVESTER);
		}
		if (element == manager.info) {
			Activate_Information();
		}
		
		if (element == entity_ui.button_1 && selected->type == EntityType::TOWNHALL) {
			Activate_Coin_Cost();
			coin_cost = 10;
		}

		if (element == entity_ui.button_2 && selected->type == EntityType::TOWNHALL) {
			if (App->expl->information_mode)
				App->expl->Show_Label(Text::QUEST);
		}

		if (element == quest_selector.Quest_1) {
			if (App->expl->information_mode)
				App->expl->Show_Label(Text::SELECT_QUEST);
		}

		if (element == quest_selector.Quest_2) {
			if (App->expl->information_mode)
				App->expl->Show_Label(Text::SELECT_QUEST);
		}

		if (element == quest_selector.Quest_3) {
			if (App->expl->information_mode)
				App->expl->Show_Label(Text::SELECT_QUEST);
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

void j1InGameUI::Activate_Resource_Menu()
{
	resources.Resources->enabled = true;
}

void j1InGameUI::Deactivate_Resource_Menu()
{
	resources.Resources->enabled = false;
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
		scroll->Button->map_position.x = 126 + (235 * percentage);
		break;
	case Material::WOOD:
		scroll->Button->map_position.x = 126 + (235 * percentage) * 4;
		break;
	case Material::METAL:
		scroll->Button->map_position.x = 126 + (235 * percentage) * 10;
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

void j1InGameUI::Change_Image_Label(j1Entity* entity)
{
	if(entity != nullptr)
	switch (entity->type)
	{
	case EntityType::BOAT:
		entity_ui.name->ChangeLabel(names::BOAT);
		entity_ui.image->texture = App->gui->Load_Texture(TEXTURE::BOAT_IMAGE);
		break;
	case EntityType::BOATHOUSE:
		entity_ui.name->ChangeLabel(names::BOATHOUSE);
		entity_ui.image->texture = App->gui->Load_Texture(TEXTURE::BUILDING_IMAGE);
		break;
	case EntityType::HARVESTER:
		entity_ui.name->ChangeLabel(names::HARVESTER);
		entity_ui.image->texture = App->gui->Load_Texture(TEXTURE::HARVESTER);
		break;
	case EntityType::TOWNHALL:
		entity_ui.name->ChangeLabel(names::TOWNHALL);
		entity_ui.image->texture = App->gui->Load_Texture(TEXTURE::TOWNHALL);
		break;
		case EntityType::STORAGE:
		entity_ui.name->ChangeLabel(names::STORAGE);
		entity_ui.image->texture = App->gui->Load_Texture(TEXTURE::STORAGE);
		break;
	case EntityType::BALLOON:
		entity_ui.name->ChangeLabel(names::BALLOON);
		entity_ui.image->texture = App->gui->Load_Texture(TEXTURE::BALLOON);
		break;
	case EntityType::CARRIER:
		entity_ui.name->ChangeLabel(names::CARRIER);
		entity_ui.image->texture = App->gui->Load_Texture(TEXTURE::CARRIER);
		break;
		case EntityType::NONE:
		entity_ui.name->ChangeLabel(names::NO_NAME);
		entity_ui.image->texture = nullptr;
		break;
	}
}

void j1InGameUI::Change_Image_Label_Trader(j1Entity* entity)
{
	
	if (entity != nullptr)
		switch (entity->type)
		{
		case EntityType::BOAT:
			Trader_label->ChangeLabel(names::BOAT);
			Trader_image->texture = App->gui->Load_Texture(TEXTURE::BOAT_IMAGE);
			break;
		case EntityType::BOATHOUSE:
			Trader_label->ChangeLabel(names::BOATHOUSE);
			Trader_image->texture = App->gui->Load_Texture(TEXTURE::BUILDING_IMAGE);
			break;
		case EntityType::HARVESTER:
			Trader_label->ChangeLabel(names::HARVESTER);
			Trader_image->texture = App->gui->Load_Texture(TEXTURE::HARVESTER);
			break;
		case EntityType::TOWNHALL:
			Trader_label->ChangeLabel(names::TOWNHALL);
			Trader_image->texture = App->gui->Load_Texture(TEXTURE::TOWNHALL);
			break;
		case EntityType::STORAGE:
			Trader_label->ChangeLabel(names::STORAGE);
			Trader_image->texture = App->gui->Load_Texture(TEXTURE::STORAGE);
			break;
		case EntityType::BALLOON:
			Trader_label->ChangeLabel(names::BALLOON);
			Trader_image->texture = App->gui->Load_Texture(TEXTURE::BALLOON);
			break;
		case EntityType::CARRIER:
			Trader_label->ChangeLabel(names::CARRIER);
			Trader_image->texture = App->gui->Load_Texture(TEXTURE::CARRIER);
			break;
		case EntityType::NONE:
			Trader_label->ChangeLabel(names::NO_NAME);
			Trader_image->texture = nullptr;
			break;
		}
}

void j1InGameUI::Deactivate_Entity_UI()
{
	entity_ui.image->enabled = false;
	entity_ui.name->enabled = false;
	entity_ui.trade->enabled = false;
	entity_ui.button_1->enabled = false;
	entity_ui.button_2->enabled = false;
	entity_ui.button_3->enabled = false;
	entity_ui.button_4->enabled = false;
	entity_ui.button_5->enabled = false;
}

void j1InGameUI::Deactivate_Entity_Buttons()
{
	entity_ui.trade->enabled = false;
	entity_ui.button_1->enabled = false;
	entity_ui.button_2->enabled = false;
	entity_ui.button_3->enabled = false;
	entity_ui.button_4->enabled = false;
	entity_ui.button_5->enabled = false;
}

bool j1InGameUI::Cost_Function(j1Entity* entity, int cotton, int wood, int metal)
{
	vector<j1Entity*>* storages = entity->GetStorages();
	if (storages == nullptr)
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
	else
	{
		int total_cotton = 0;
		int wood_cotton = 0;
		int metal_cotton = 0;
		for (vector<j1Entity*>::iterator itr = storages->begin(); itr != storages->end(); itr++)
		{
			j1Entity* s = *itr;

			total_cotton += s->load.cotton;
			wood_cotton += s->load.wood;
			metal_cotton += s->load.metal;

			if (total_cotton >= cotton && wood_cotton >= wood && metal_cotton >= metal)
				break;
			else if (itr == storages->end() - 1)
				return false;
		}
		for (vector<j1Entity*>::iterator itr = storages->begin(); itr != storages->end(); itr++)
		{
			j1Entity* s = *itr;

			if (s->load.cotton != 0)
			{
				s->load.cotton = s->load.cotton - cotton;
				if (s->load.cotton < 0)
				{
					cotton = -s->load.cotton;
					s->load.cotton = 0;
				}
				cotton = 0;
			}
			if (s->load.wood != 0)
			{
				s->load.wood = s->load.wood - wood;
				if (s->load.wood < 0)
				{
					wood = -s->load.wood;
					s->load.wood = 0;
				}
				wood = 0;
			}
			if (s->load.metal != 0)
			{
				s->load.metal = s->load.metal - metal;
				if (s->load.metal < 0)
				{
					metal = -s->load.metal;
					s->load.metal = 0;
				}
				metal = 0;
			}
			if (cotton + wood + metal == 0)
				return true;
		}
	}
	return false;
}

void j1InGameUI::Manage_Entity_UI(j1Entity* entity)
{
	if (entity != nullptr) {

		Activate_Manager();

		entity_ui.name->enabled = true;
		entity_ui.image->enabled = true;
		entity_ui.trade->enabled = true;

		switch (entity->type)
		{
		case EntityType::BOATHOUSE:
			Change_Image_Label(entity);


			entity_ui.button_1->texture = App->gui->Load_Texture(TEXTURE::BOAT_IMAGE);
			entity_ui.button_1->enabled = true;

			entity_ui.button_2->texture = App->gui->Load_Texture(TEXTURE::CARRIER);
			entity_ui.button_2->enabled = true;

			entity_ui.button_3->texture = App->gui->Load_Texture(TEXTURE::BALLOON);
			entity_ui.button_3->enabled = true;

			entity_ui.button_4->enabled = false;
			entity_ui.button_5->enabled = false;
			if (in_trading == true) {
				Deactivate_Entity_Buttons();
			}
			break;

		case EntityType::BOAT:
			Change_Image_Label(entity);
			entity_ui.button_1->enabled = false;
			entity_ui.button_2->enabled = false;
			entity_ui.button_3->enabled = false;
			entity_ui.button_4->enabled = false;
			entity_ui.button_5->enabled = false;
			if (in_trading == true) {
				Deactivate_Entity_Buttons();
			}
			break;

		case EntityType::HARVESTER:
			Change_Image_Label(entity);

			entity_ui.button_1->texture = App->gui->Load_Texture(TEXTURE::BUILDING_IMAGE);
			entity_ui.button_1->enabled = true;

			entity_ui.button_2->texture = App->gui->Load_Texture(TEXTURE::STORAGE);
			entity_ui.button_2->enabled = true;

			entity_ui.button_3->enabled = false;
			entity_ui.button_4->enabled = false;

			entity_ui.button_5->texture = App->gui->Load_Texture(TEXTURE::AUTOMATIC);
			entity_ui.button_5->enabled = true;

			if (in_trading == true) {
				Deactivate_Entity_Buttons();
			}
			break;

		case EntityType::STORAGE:
			Change_Image_Label(entity);

			entity_ui.button_1->texture = App->gui->Load_Texture(TEXTURE::HARVESTER);
			entity_ui.button_1->enabled = true;

			entity_ui.button_2->enabled = false;
			entity_ui.button_3->enabled = false;
			entity_ui.button_4->enabled = false;
			entity_ui.button_5->enabled = false;
			if (in_trading == true) {
				Deactivate_Entity_Buttons();
			}
			break;

		case EntityType::TOWNHALL:
			Change_Image_Label(entity);

			entity_ui.button_1->texture = App->gui->Load_Texture(TEXTURE::LVLUP);
			entity_ui.button_1->enabled = true;

			entity_ui.button_2->texture = App->gui->Load_Texture(TEXTURE::QUEST);
			entity_ui.button_2->enabled = true;

			entity_ui.button_3->enabled = false;
			entity_ui.button_4->enabled = false;
			entity_ui.button_5->enabled = false;
			if (in_trading == true) {
				Deactivate_Entity_Buttons();
			}
			break;

		case EntityType::BALLOON:
		Change_Image_Label(entity);
		entity_ui.button_1->enabled = false;
		entity_ui.button_2->enabled = false;
		entity_ui.button_3->enabled = false;
		entity_ui.button_4->enabled = false;
		entity_ui.button_5->enabled = false;
		if (in_trading == true) {
			Deactivate_Entity_Buttons();
		}
		break;

		case EntityType::CARRIER:
			Change_Image_Label(entity);
			entity_ui.button_1->enabled = false;
			entity_ui.button_2->enabled = false;
			entity_ui.button_3->enabled = false;
			entity_ui.button_4->enabled = false;
			entity_ui.button_5->enabled = false;
			if (in_trading == true) {
				Deactivate_Entity_Buttons();
			}
			break;


		case EntityType::NONE:
			break;
		}
	}
	else {
		Deactivate_Manager();
		Deactivate_Entity_UI();
		Deactivate_Entity_Buttons();
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