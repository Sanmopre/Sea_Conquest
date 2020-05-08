#include "j1App.h"
#include "j1GUIElements.h"

#include "j1Input.h"
#include "j1GUI.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Player.h"
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


	boathouse.first = "BUILD BOATHOUSE BUTTON:";
	boathouse.second = "";
	boathouse.third = "Build a boathouse";
	boathouse.forth = "from harvester.";

	storage.first = "BUILD STORAGE BUTTON";
	storage.second = "";
	storage.third = "Build a storage from";
	storage.forth = "harvester.";

	harvester.first = "CREATE HARVESTER BUTTON";
	harvester.second = "";
	harvester.third = "Create a level one";
	harvester.forth = "harvester.";

	quest.first = "QUEST MANAGER:";
	quest.second = "";
	quest.third = "Open quest manager";
	quest.forth = "to select a quest";

	select_quest.first = "SELECT QUEST:";
	select_quest.second = "";
	select_quest.third = "Click to select this quest.";
	select_quest.forth = "";

	trader.first = "EXCHANGE BUTTON:";
	trader.second = "";
	trader.third = "Click to open the";
	trader.forth = "exchange manager.";

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

bool j1Explanation::Change_Label_Text(Explanation explanation)
{
	bool changed = false;
	first->ChangeLabel(explanation.first);
	second->ChangeLabel(explanation.second);
	third->ChangeLabel(explanation.third);
	forth->ChangeLabel(explanation.forth);
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
		Change_Label_Text(boat);
		Activate_Explanation();
		break;
	case Text::BOATHOUSE:
		Change_Label_Text(boathouse);
		Activate_Explanation();
		break;
	case Text::HARVESTER:
		Change_Label_Text(harvester);
		Activate_Explanation();
		break;
	case Text::STORAGE:
		Change_Label_Text(storage);
		Activate_Explanation();
		break;
	case Text::QUEST:
		Change_Label_Text(quest);
		Activate_Explanation();
		break;
	case Text::SELECT_QUEST:
		Change_Label_Text(select_quest);
		Activate_Explanation();
		break;
	case Text::TRADE:
		Change_Label_Text(trader);
		Activate_Explanation();
		break;
	case Text::NONE:
		break;
	}
}


