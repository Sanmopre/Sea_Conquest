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
	return true;
}

bool j1Explanation::Update(float dt)
{
	return true;
}
 
bool j1Explanation::PostUpdate()
{
	return true;
}

void j1Explanation::Show_Information(Text text)
{
	j1Element* text_label = nullptr;

	explanation.Image->enabled = true;
	Update_Position(explanation.Image);
	text_label = Change_Label(text);
	Update_Position(text_label);
}

void j1Explanation::Update_Position(j1Element* element)
{
	int x, y;
	App->input->GetMousePosition(x, y);
	element->map_position.x = x + 30;
	element->map_position.y = y + 30;
}

j1Element* j1Explanation::Change_Label( Text text)
{
	j1Element* text_label = nullptr;
	switch(text)
	{
	case Text::NONE:
		text_label = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 150,555 }, { 0,0 }, true, false, { 0,0,40,40 }, "STORAGE", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
		break;
	}

	return text_label;
}
