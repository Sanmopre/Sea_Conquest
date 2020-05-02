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

void j1Explanation::Update_Position_(j1Element* element)
{
	int x, y;
	App->input->GetMousePosition(x, y);
	element->map_position.x = x + 30;
	element->map_position.y = y + 30;
}

void j1Explanation::Change_Label(j1Element* element, Text text)
{
}
