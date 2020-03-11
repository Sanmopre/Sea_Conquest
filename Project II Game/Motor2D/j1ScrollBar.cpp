#include "j1GUIElements.h"
#include "j1GUI.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Audio.h"


j1ScrollBar::j1ScrollBar(SCROLL_TYPE TypeInput) {

	this->type = GUItype::GUI_SCROLLBAR;
	Type = TypeInput;
}

j1ScrollBar::~j1ScrollBar() {

}

bool j1ScrollBar::Start()
{
	Button = App->gui->AddElement(GUItype::GUI_BUTTON, this, map_position, inside_position, true, true, { 432, 36, 14 , 16 }, nullptr, this->listener, true, false);
	Button->map_position.y = map_position.y - Button->rect.h / 2 + this->rect.h / 2;
	Value = 0;

	if (this->Type == SCROLL_TYPE::SCROLL_MUSIC)
	{

	}

	return true;
}

bool j1ScrollBar::PreUpdate()
{

	Button->enabled = enabled;
	above = OnAbove();

	return true;
}

bool j1ScrollBar::Update(float dt)
{
	if (interactable) {
		if (above)
		{
			if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
				OnClick();
		}
	}

	return true;
}

bool j1ScrollBar::PostUpdate()
{
	ScrollLimits();
	Value = -((float(-Button->inside_position.x) / (float(-this->rect.w) + float(Button->rect.w))) * 128);

	if (this->Type == SCROLL_TYPE::SCROLL_MUSIC)
	{

	}

	if (enabled)
		Draw();

	return true;
}



bool j1ScrollBar::CleanUp()
{
	return true;
}


void j1ScrollBar::ScrollLimits() {

	if (Button->inside_position.x > 0)
	{
		Button->inside_position.x = 0;

		Button->map_position.x = Button->parent->map_position.x - Button->inside_position.x;

	}
	else if (Button->inside_position.x < (-this->rect.w + Button->rect.w))
	{
		Button->inside_position.x = -this->rect.w + Button->rect.w;

		Button->map_position.x = Button->parent->map_position.x - Button->inside_position.x;

	}

}