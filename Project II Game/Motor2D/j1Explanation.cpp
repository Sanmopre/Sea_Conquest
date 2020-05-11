#include "j1App.h"
#include "j1GUIElements.h"

#include "j1Input.h"
#include "j1GUI.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Player.h"
#include "j1Font.h"
#include "j1Explanation.h"

j1Explanation::j1Explanation()
{
}

j1Explanation::~j1Explanation()
{
}

bool j1Explanation::Awake(pugi::xml_node& config)
{
	return true;
}

bool j1Explanation::Start()
{
	explanation.Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 0,0 }, { 0,0 }, true, true, { 0, 0,200,130 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::EXPLANATION);
	information_image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 10,200 }, { 0,0 }, true, true, { 0, 0,200,80 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::INFORMATION_IMAGE);

	first = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 150,150 }, { 0,0 }, true, true, { 0,0,40,40 }, " ", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	second = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 150,180 }, { 0,0 }, true, true, { 0,0,40,40 }, " ", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	third = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 150,220 }, { 0,0 }, true, true, { 0,0,40,40 }, " ", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	forth = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 150,250 }, { 0,0 }, true, true, { 0,0,40,40 }, " ", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	//BOAT EXPLANAITON
	boat.first = "CREATE BOAT BUTTON:";
	boat.second = "";
	boat.third = "Create a level one";
	boat.forth = "boat.";


	boathouse_t.first = App->font->Print( "BUILD BOATHOUSE BUTTON:");
	boathouse_t.second = App->font->Print("");
	boathouse_t.third = App->font->Print("Build a boathouse");
	boathouse_t.forth = App->font->Print("from harvester.");

	boat_t.first = App->font->Print("CREATE BOAT BUTTON:");
	boat_t.second = App->font->Print("");
	boat_t.third = App->font->Print("Create a level one");
	boat_t.forth = App->font->Print("boat");

	storage_t.first = App->font->Print("BUILD STORAGE BUTTON");
	storage_t.second = App->font->Print("");
	storage_t.third = App->font->Print("Build a storage from");
	storage_t.forth = App->font->Print("harvester.");

	harvester_t.first = App->font->Print("CREATE HARVESTER BUTTON");
	harvester_t.second = App->font->Print("");
	harvester_t.third = App->font->Print("Create a level one");
	harvester_t.forth = App->font->Print("harvester.");

	quest_t.first = App->font->Print("QUEST MANAGER:");
	quest_t.second = App->font->Print("");
	quest_t.third = App->font->Print("Open quest manager");
	quest_t.forth = App->font->Print("to select a quest");

	select_quest_t.first = App->font->Print("SELECT QUEST:");
	select_quest_t.second = App->font->Print("");
	select_quest_t.third = App->font->Print("Click to select this quest.");
	select_quest_t.forth = App->font->Print("");

	trader_t.first = App->font->Print("EXCHANGE BUTTON:");
	trader_t.second = App->font->Print("");
	trader_t.third = App->font->Print("Click to open the");
	trader_t.forth = App->font->Print("exchange manager.");

	return true;
}

bool j1Explanation::Update(float dt)
{



	//INFORMATION MODE ACTIVE 
	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN) {
		information_mode = !information_mode;
	}
	information_image->enabled = information_mode;
	



	//MANAGES EXPLANATION MODE ACTIVE
	if (explanation_active == false) {
		Deactivate_Explanation();
	}
	explanation_active = false;





	return true;
}
 
bool j1Explanation::PostUpdate()
{
	return true;
}


void j1Explanation::Update_Position(j1Element* element, int pos_x, int pos_y)
{
	int x, y;
	App->input->GetMousePosition(x, y);
	element->map_position.x = x + 50 + pos_x;
	element->map_position.y = y - 100 + pos_y;
}

bool j1Explanation::Change_Label_Text(Explanation_T explanation)
{
	bool changed = false;
	first->texture = explanation.first;
	second->texture = explanation.second;
	third->texture = explanation.third;
	forth->texture = explanation.forth;
	changed = true;
	return changed;
}

void j1Explanation::Activate_Explanation()
{
	explanation.Image->enabled = true;
	first->enabled = true;
	second->enabled = true;
	third->enabled = true;
	forth->enabled = true;

	Update_Position(first, 15, 18);
	Update_Position(second , 15 , 40);
	Update_Position(third , 15 , 70);
	Update_Position(forth , 15 , 100);
	Update_Position(explanation.Image);
}

void j1Explanation::Deactivate_Explanation()
{
	explanation.Image->enabled = false;
	first->enabled = false;
	second->enabled = false;
	third->enabled = false;
	forth->enabled = false;
}


void j1Explanation::Show_Label(Text text)
{
	explanation_active = true;
	switch(text)
	{
	case Text::BOAT:
		Change_Label_Text(boat_t);
		Activate_Explanation();
		break;
	case Text::BOATHOUSE:
		Change_Label_Text(boathouse_t);
		Activate_Explanation();
		break;
	case Text::HARVESTER:
		Change_Label_Text(harvester_t);
		Activate_Explanation();
		break;
	case Text::STORAGE:
		Change_Label_Text(storage_t);
		Activate_Explanation();
		break;
	case Text::QUEST:
		Change_Label_Text(quest_t);
		Activate_Explanation();
		break;
	case Text::SELECT_QUEST:
		Change_Label_Text(select_quest_t);
		Activate_Explanation();
		break;
	case Text::TRADE:
		Change_Label_Text(trader_t);
		Activate_Explanation();
		break;
	case Text::NONE:
		break;
	}
}


