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
	explanation.Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 0,0 }, { 0,0 }, true, true, { 0, 0,200,200 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::EXPLANATION);
	first = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 150,150 }, { 0,0 }, true, true, { 0,0,40,40 }, " ", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	second = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 150,180 }, { 0,0 }, true, true, { 0,0,40,40 }, " ", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	third = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 150,220 }, { 0,0 }, true, true, { 0,0,40,40 }, " ", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	forth = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 150,250 }, { 0,0 }, true, true, { 0,0,40,40 }, " ", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	//BOAT EXPLANAITON
	boat.first = "This unit is";
	boat.second = "a really cool";
	boat.third = "unit OMEGALUL";
	boat.forth = "read this = SIMP";


	boathouse.first = "NANANANA";
	boathouse.second = "aSDWSDSD";
	boathouse.third = "uniEDEDEDELUL";
	boathouse.forth = "readEDEDEEDE= SIMP";
	
	return true;
}

bool j1Explanation::Update(float dt)
{
	//DEBUG ONLY
	/*
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT) {
		Show_Label(Text::BOAT);
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_REPEAT) {
		Show_Label(Text::BOATHOUSE);
	}
*/
	return true;
}
 
bool j1Explanation::PostUpdate()
{
	return true;
}


void j1Explanation::Update_Position(j1Element* element)
{
	int x, y;
	App->input->GetMousePosition(x, y);
	element->map_position.x = x + 30;
	element->map_position.y = y + 30;
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

	Update_Position(first);
	Update_Position(second);
	Update_Position(third);
	Update_Position(forth);
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
		Change_Label_Text(boat);
		Activate_Explanation();
		break;
	case Text::STORAGE:
		Change_Label_Text(boat);
		Activate_Explanation();
		break;
	case Text::NONE:
		break;
	}
}


