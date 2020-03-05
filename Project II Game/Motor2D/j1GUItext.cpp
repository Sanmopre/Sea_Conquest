#include "j1GUItext.h"
#include "j1App.h"


j1GUItext::j1GUItext() {

	this->type = GUItype::GUI_TEXT;
}

j1GUItext::~j1GUItext() {

}


bool j1GUItext::Awake(pugi::xml_node&)
{

	return true;
}

bool j1GUItext::PreUpdate()
{

	return true;
}

bool j1GUItext::PostUpdate()
{
	if (enabled)
		Draw();

	return true;
}

bool j1GUItext::CleanUp()
{
	return true;
}

