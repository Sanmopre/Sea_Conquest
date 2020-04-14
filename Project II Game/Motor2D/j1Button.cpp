#include "j1App.h"
#include "j1GUIElements.h"

#include "j1Input.h"
#include "j1GUI.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"


j1Button::j1Button() {
	this->type = GUItype::GUI_BUTTON;
}

j1Button::~j1Button() {

}

bool j1Button::Start()
{
	if (textureType == TEXTURE::BUTON) {
		texture = App->gui->Load_Texture(TEXTURE::BUTON);
		texture_hover = App->gui->Load_Texture(TEXTURE::BUTON_HOVER);
	}

	if (textureType == TEXTURE::TRADE)
		texture = App->gui->Load_Texture(TEXTURE::TRADE);

	if (textureType == TEXTURE::OPTIONS)
		texture = App->gui->Load_Texture(TEXTURE::OPTIONS);

	if (textureType == TEXTURE::BOAT_IMAGE)
		texture = App->gui->Load_Texture(TEXTURE::BOAT_IMAGE);

	if (textureType == TEXTURE::TRADER_DERECHA)
		texture = App->gui->Load_Texture(TEXTURE::TRADER_DERECHA);

	if (textureType == TEXTURE::TRADER_IZQUIERDA)
		texture = App->gui->Load_Texture(TEXTURE::TRADER_IZQUIERDA);

	if (textureType == TEXTURE::NEXT)
		texture = App->gui->Load_Texture(TEXTURE::NEXT);

	if (textureType == TEXTURE::PREV)
		texture = App->gui->Load_Texture(TEXTURE::PREV);

	if (textureType == TEXTURE::SCROLL)
		texture = App->gui->Load_Texture(TEXTURE::SCROLL);

	if (textureType == TEXTURE::LINK)
		texture = App->gui->Load_Texture(TEXTURE::LINK);
	
	if (textureType == TEXTURE::BOTON_SCROLL)
		texture = App->gui->Load_Texture(TEXTURE::BOTON_SCROLL);
	
	if (textureType == TEXTURE::COTTON)
		texture = App->gui->Load_Texture(TEXTURE::COTTON);

	if (textureType == TEXTURE::WOOD)
		texture = App->gui->Load_Texture(TEXTURE::WOOD);

	if (textureType == TEXTURE::METAL)
		texture = App->gui->Load_Texture(TEXTURE::METAL);

	if (textureType == TEXTURE::HARVESTER)
		texture = App->gui->Load_Texture(TEXTURE::HARVESTER);


	if (text != nullptr)
		label = App->gui->AddElement(GUItype::GUI_LABEL, this, map_position, inside_position, true, true, { 0,0,0,0 }, text);

	return true;
}

bool j1Button::PreUpdate()
{
	if (label != nullptr)
		label->enabled = enabled;

	above = OnAbove();

	return true;
}

bool j1Button::Update(float dt)
{
	if (interactable)
	{
		if (above)
		{
			if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
				OnClick();

			if (App->input->GetMouseButtonDown(3) == KEY_DOWN)
				OnLeftClick();

			if (App->input->GetMouseButtonDown(1) == KEY_REPEAT)
			{
				if (X_drag || Y_drag)
					dragging = true;

				iPoint mouseClick = { 0,0 };
				App->input->GetMousePosition(mouseClick.x, mouseClick.y);
				drag = { mouseClick.x - ((int)this->map_position.x), mouseClick.y - ((int)this->map_position.y) };

			}

		}

		if (dragging) {

			if (App->input->GetMouseButtonDown(1) == KEY_IDLE || App->input->GetMouseButtonDown(1) == KEY_UP)
				dragging = false;
			else
			{
				Dragging();
				MovingIt(dt);
			}
		}
	}

	if (enabled) {


		if (textureType == TEXTURE::BUTON) {
			if (above && interactable)
			{
				App->render->AddBlitEvent(5, texture_hover, map_position.x - App->render->camera.x , map_position.y - App->render->camera.y, rect, false, true, 0u, 0u, 0u, 255, true);
			}
			else {
				App->render->AddBlitEvent(5, texture, map_position.x - App->render->camera.x , map_position.y - App->render->camera.y, rect, false, true, 0u, 0u, 0u, 255, true);
			}
		}
		else if (textureType == TEXTURE::BOTON_SCROLL) {

			//LIMITING THE SCROLL BAR BUTTON VISUALY
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (inside_position.x > 0) {
				App->render->AddBlitEvent(5, texture, map_position.x - App->render->camera.x + inside_position.x, map_position.y - App->render->camera.y, rect, false, true, 0u, 0u, 0u, 255, true);
			}
			else if (inside_position.x < -235) {
				App->render->AddBlitEvent(5, texture, map_position.x - App->render->camera.x + inside_position.x + 235, map_position.y - App->render->camera.y, rect, false, true, 0u, 0u, 0u, 255, true);
			}
			else 
				App->render->AddBlitEvent(5, texture, map_position.x - App->render->camera.x , map_position.y - App->render->camera.y, rect, false, true, 0u, 0u, 0u, 255, true);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		
		}
		else{
			App->render->AddBlitEvent(5, texture, map_position.x - App->render->camera.x, map_position.y - App->render->camera.y, rect, false, true, 0u, 0u, 0u, 255, true);

		}




		}
	
	return true;
}

bool j1Button::PostUpdate()
{

	return true;
}

bool j1Button::CleanUp()
{
	return true;
}

void j1Button::Dragging()
{

}

void j1Button::MovingIt(float dt)
{

	iPoint mouse_pos = { 0,0 };
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);

	fPoint currentPos = this->map_position;


	if (X_drag)
		this->map_position.x += ((mouse_pos.x - this->map_position.x) - drag.x);

	if (Y_drag)
		this->map_position.y += ((mouse_pos.y - this->map_position.y) - drag.y);


	if (parent != nullptr)
	{
		if (X_drag)
			this->inside_position.x += currentPos.x - this->map_position.x;

		if (Y_drag)
			this->inside_position.y += currentPos.y - this->map_position.y;

		if (X_drag)
			this->map_position.x = parent->map_position.x - inside_position.x;

		if (Y_drag)
			this->map_position.y = parent->map_position.y - inside_position.y;
	}

}

void j1Button::OnRelease()
{

}
