#ifndef __j1INGAMEUI_H__
#define __j1INGAMEUI_H__

#include "j1Module.h"
#include "SDL/include/SDL_rect.h"

struct Game_Menu {
	j1Element* Menu_button;
	j1Element* Title;
	j1Element* Image;
	j1Element* Exit_button;
	j1Element* Return_button;
	j1Element* Resume_button;
	j1Element* Save;
	j1Element* Load;
};

struct Unit_Manager_UI {
	j1Element* button_next;
	j1Element* buton_prev;
	j1Element* image;
	j1Element* entity_type_Image;
	j1Element* entity_name_boat;
	j1Element* entity_name_boathouse;
};

struct In_Game_Basics {
	j1Element* Image;
	j1Element* Resources;
};


struct Building_Menu {
	j1Element* Boat_Building_Button;
};

struct SDL_Texture;
class j1Entity;

class j1InGameUI : public j1Module
{
public:

	j1InGameUI();
	~j1InGameUI();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void Add_UI();
	void Activate_Menu();
	void Activate_Building_Menu();
	void Deactivate_Building_Menu();
	//void Name_Selector(EntityType type = EntityType::NONE);

	void Activate_Manager();
	void Deactivate_Manager();
	void GUI_Event_Manager(GUI_Event type, j1Element* element);

private:
	//positions
	float MiddleScreenW;
	float MiddleScreenH;
	float width;
	

	//resources
	int type_0;
	int type_1;
	int type_2;

	char text_type_0[10];
	char text_type_1[10];
	char text_type_2[10];


	//menu ui
	Game_Menu menu;
	Building_Menu building;
	Unit_Manager_UI manager;
	In_Game_Basics basics;

	j1Entity* entity_ui = nullptr;

	SDL_Texture* resources;
	SDL_Texture* UI_Image;
	//SDL_Rect texture_rect{ 0,0,1280,200 };
	SDL_Rect texture_rect_1{ 0,0,400,30 };
	int font_name = -1;
	bool quit = false;
};

#endif // __j1WINDOW_H__