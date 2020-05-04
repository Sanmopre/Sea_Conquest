#include "j1App.h"
#include "j1GUIElements.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1GUI.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Player.h"
#include "j1DialogSystem.h"

j1DialogSystem::j1DialogSystem()
{
}

j1DialogSystem::~j1DialogSystem()
{
}

bool j1DialogSystem::Awake(pugi::xml_node& config)
{

	return true;
}

bool j1DialogSystem::Start()
{
	pirate.Label = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 990,200 }, { 0,0 }, true, false, { 0,0,40,40 }, "YOU ARE NOT PREPARED!!", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	pirate.Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 900,170 }, { 0,0 }, true, false, { 0, 0,300,80 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::DIALOG);
	
	pirate_fun.Label = App->gui->AddElement(GUItype::GUI_LABEL, nullptr, { 990,200 }, { 0,0 }, true, false, { 0,0,40,40 }, "THIS WILL BE FUN!!", this, false, false, SCROLL_TYPE::SCROLL_NONE, true);
	pirate_fun.Image = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 900,170 }, { 0,0 }, true, false, { 0, 0,300,80 }, "", this, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::DIALOG);

	return true;
}

bool j1DialogSystem::Update(float dt)
{


	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN && show_dialog == false)
	{
		Display_Dialog(Dialog::PIRATE);
	}

	if (App->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN && show_dialog == false)
	{
		Display_Dialog(Dialog::FUN);
	}

		timer.counter += dt;
		if (timer.counter >= timer.iterations) {
			show_dialog = false;
			in_timer = false;
		}
		else {
			in_timer = true;
		}
	



	if (show_dialog == false) 
		Hide_Dialogs();
	


	return true;
}

void j1DialogSystem::Display_Dialog(Dialog dialog)
{
	timer.counter = 0;

	switch (dialog) {
	case Dialog::NONE:
		break;
	case Dialog::PIRATE:
		App->audio->PlayFx(App->audio->you_are_not_prepared, 0);
		pirate.Image->enabled = true;
		pirate.Label->enabled = true;
		Display_timer = 4.0f;
		timer.iterations = Display_timer;
		show_dialog = true;
		break;
	case Dialog::FUN:
		App->audio->PlayFx(App->audio->this_will_be_fun, 0);
		pirate_fun.Image->enabled = true;
		pirate_fun.Label->enabled = true;
		Display_timer = 2.8f;
		timer.iterations = Display_timer;
		show_dialog = true;
		break;
	}
}

void j1DialogSystem::Hide_Dialogs()
{
	pirate.Image->enabled = false;
	pirate.Label->enabled = false;
	pirate_fun.Image->enabled = false;
	pirate_fun.Label->enabled = false;

}
