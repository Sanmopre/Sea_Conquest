#include "j1GUIElements.h"
#include "j1GUI.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Font.h"
#include "j1Window.h"

j1Label::j1Label()
{
	this->type = GUItype::GUI_LABEL;
}

j1Label::~j1Label() {

}

bool j1Label::ChangeLabel(names name)
{
	texture = App->font->Change_Name(name);
	return true;
}

bool j1Label::Start()
{
	//font_name = App->fonts->Load("textures/ui/font.png", "ABCDEFGHIJKLMNOPQRSTUWYZ0123456789-= ", 1);
	texture = App->font->Print(text);
	return true;
}


bool j1Label::PreUpdate()
{

	return true;
}

bool j1Label::Update(float dt)
{

	SDL_Rect rect_font = { 0,0,0,0 };
	SDL_QueryTexture(texture, NULL, NULL, &rect_font.w, &rect_font.h);
	if (enabled)
		App->render->AddBlitEvent(5, texture, map_position.x + inside_position.x, map_position.y + inside_position.y, rect_font, false, true, 0, 0, 0, 255, true);


	return true;
}

bool j1Label::PostUpdate()
{

	return true;
}

bool j1Label::CleanUp()
{
	App->tex->UnLoad(texture);
	return true;
}