#include "j1App.h"
#include "j1GUI.h"

#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"


j1GUI::j1GUI() : j1Module()
{
	name.create("gui");
}

// Destructor
j1GUI::~j1GUI()
{}


bool j1GUI::Awake(pugi::xml_node& config)
{
	bool ret = true;

	return ret;
}


bool j1GUI::Start()
{

	return true;
}


bool j1GUI::PreUpdate()
{
	bool ret = true;
	p2List_item<j1Element*>* tmp = GUI_ELEMENTS.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->PreUpdate();
		tmp = tmp->next;
	}

	return ret;

}


bool j1GUI::Update(float dt)
{

	bool ret = true;
	p2List_item<j1Element*>* tmp = GUI_ELEMENTS.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->Update(dt);
		tmp = tmp->next;
	}

	return ret;

}


bool j1GUI::PostUpdate()
{

	bool ret = true;

	p2List_item<j1Element*>* tmp = GUI_ELEMENTS.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->PostUpdate();
		tmp = tmp->next;
	}
	return ret;

}



bool j1GUI::CleanUp()
{
	LOG("Freeing GUI");

	for (p2List_item<j1Element*>* item = GUI_ELEMENTS.start; item; item = item->next)
	{
		item->data->CleanUp();
	}
	GUI_ELEMENTS.clear();
	return true;
}

SDL_Texture* j1GUI::Load_Texture(TEXTURE textureType)
{
	switch (textureType)
	{
	case TEXTURE::BUTON:
		texture_load = App->tex->Load("textures/BOTON.png");
		break;
	case TEXTURE::IMAGE:
		texture_load = App->tex->Load("textures/image.png");
		break;
	case TEXTURE::MANAGER_IMAGE:
		texture_load = App->tex->Load("textures/MANAGER_IMAGE.png");
		break;
	case TEXTURE::NEXT:
		texture_load = App->tex->Load("textures/next.png");
		break;
	case TEXTURE::PREV:
		texture_load = App->tex->Load("textures/prev.png");
		break;
	case TEXTURE::OPTIONS:
		texture_load = App->tex->Load("textures/BOTON_2.png");
		break;
	case TEXTURE::BUTON_HOVER:
		texture_load = App->tex->Load("textures/BOTON_1.png");
		break;
	case TEXTURE::MAIN_IMAGE:
		texture_load = App->tex->Load("textures/UIimage.png");
		break;
	case TEXTURE::RESOURCES_IMAGE:
		texture_load = App->tex->Load("textures/Recursos.png");
		break;
	case TEXTURE::BUILDING_IMAGE:
		texture_load = App->tex->GetTexture("boathouse-icon", 0,0);
		break;
	case TEXTURE::BOAT_IMAGE:
		texture_load = App->tex->GetTexture("boat-icon", 0, 0);
		break;
	case TEXTURE::SCROLL:
		texture_load = App->tex->Load("textures/scroll.png");
		break;
	case TEXTURE::TRADE:
		texture_load = App->tex->Load("textures/trade.png");
		break;
	case TEXTURE::HARVESTER:
		texture_load = App->tex->GetTexture("harvester-icon", 0, 0);
		break;
	case TEXTURE::BOTON_SCROLL:
		texture_load = App->tex->Load("textures/boton_scroll.png");
		break;
	case TEXTURE::AUDIO_IMAGE:
		texture_load = App->tex->Load("textures/audio_image.png");
		break;
	case TEXTURE::LINK:
		texture_load = App->tex->Load("textures/link.png");
		break;
	case TEXTURE::TRADER_DERECHA:
		texture_load = App->tex->Load("textures/derecha.png");
		break;
	case TEXTURE::TRADER_IZQUIERDA:
		texture_load = App->tex->Load("textures/izquierda.png");
		break;
	case TEXTURE::COTTON:
		texture_load = App->tex->Load("textures/cotton.png");
		break;
	case TEXTURE::WOOD:
		texture_load = App->tex->Load("textures/wood.png");
		break;
	case TEXTURE::METAL:
		texture_load = App->tex->Load("textures/metal.png");
		break;
	case TEXTURE::TOWNHALL:
		texture_load = App->tex->GetTexture("townhall-icon", 0, 0);
		break;
	case TEXTURE::STORAGE:
		texture_load = App->tex->GetTexture("storage-icon", 0, 0);
		break;
	case TEXTURE::COST:
		texture_load = App->tex->Load("textures/cost.png");
		break;
	case TEXTURE::GODMODE:
		texture_load = App->tex->Load("textures/godmode.png");
		break;
	case TEXTURE::INFO:
		texture_load = App->tex->Load("textures/info.png");
		break;
	case TEXTURE::INFO_IMAGE:
		texture_load = App->tex->Load("textures/info_image.png");
		break;
	case TEXTURE::QUEST_CLOSE:
		texture_load = App->tex->Load("textures/close_quest.png");
		break;
	case TEXTURE::QUEST_IMAGE_CLOSE:
		texture_load = App->tex->Load("textures/quest_close.png");
		break;
	case TEXTURE::LVLUP:
		texture_load = App->tex->GetTexture("lvlup-icon", 0, 0);
		break;
	case TEXTURE::QUEST_IMAGE_OPEN:
		texture_load = App->tex->Load("textures/quest_open.png");
		break;
	case TEXTURE::COIN:
		texture_load = App->tex->Load("textures/coin.png");
		break;
	case TEXTURE::AUTOMATIC:
		texture_load = App->tex->GetTexture("automatic-icon", 0, 0);
		break;
	case TEXTURE::COIN_COST:
		texture_load = App->tex->Load("textures/coin_cost.png");
		break;
    case TEXTURE::EXPLANATION:
		texture_load = App->tex->Load("textures/explanation.png");
		break;
	case TEXTURE::DIALOG:
		texture_load = App->tex->Load("textures/dialog.png");
		break;
	case TEXTURE::BALLOON:
		texture_load = App->tex->GetTexture("balloon-icon", 0, 0);
		break;
	case TEXTURE::SHIP:
		texture_load = App->tex->Load("textures/ship.png");
		break;
	case TEXTURE::QUEST:
		texture_load = App->tex->GetTexture("quest-icon", 0, 0);
		break;
	case TEXTURE::INFORMATION_IMAGE:
		texture_load = App->tex->Load("textures/information_image.png");
		break;
	case TEXTURE::CARRIER:
		texture_load = App->tex->GetTexture("carrier-icon", 0, 0);
		break;
	}

	return texture_load;
}



j1Element* j1GUI::AddElement(GUItype type, j1Element* parent, fPoint map_position, fPoint inside_position, bool interactable, bool enabled, SDL_Rect section, char* text, j1Module* listener, bool X_drag, bool Y_drag, SCROLL_TYPE scrollType, bool decor, TEXTURE textureType)
{

	j1Element* temp = nullptr;

	switch (type)
	{

	case GUItype::GUI_BUTTON:
		temp = new j1Button();
		break;
	case GUItype::GUI_INPUTBOX:
		temp = new j1InputBox(text);
		break;
	case GUItype::GUI_LABEL:
		temp = new j1Label();
		break;
	case GUItype::GUI_IMAGE:
		temp = new j1Image();
		break;
	case GUItype::GUI_SCROLLBAR:
		temp = new j1ScrollBar(scrollType);
		break;
	}

	if (temp)
	{
		temp->parent = parent;
		temp->map_position = map_position;
		temp->inside_position = inside_position;
		temp->listener = listener;
		temp->interactable = interactable;
		temp->X_drag = X_drag;
		temp->Y_drag = Y_drag;
		temp->decorative = decor;
		temp->enabled = enabled;
		temp->rect = section;
		temp->text = text;
		temp->textureType = textureType;
	

		GUI_ELEMENTS.add(temp)->data->Start();
	}

	return temp;
}

bool j1GUI::Save(pugi::xml_node& file) const {

	return true;
}


bool j1GUI::Load(pugi::xml_node& file) {

	return true;
}

void j1GUI::UpdatePosition(j1Element* element, fPoint position, fPoint localPosition) {
	element->map_position = position;
	element->inside_position = localPosition;
}