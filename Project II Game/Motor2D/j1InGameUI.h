#ifndef __j1INGAMEUI_H__
#define __j1INGAMEUI_H__

#include "j1Module.h"
#include "SDL/include/SDL_rect.h"
#include <vector>

struct Game_Menu {
	j1Element* Menu_button;
	j1Element* Title;
	j1Element* Image;
	j1Element* Exit_button;
	j1Element* Return_button;
	j1Element* Resume_button;
	j1Element* Save;
	j1Element* Load;
	j1Element* Scroll;
};

struct Unit_Manager_UI {
	j1Element* button_next;
	j1Element* buton_prev;
	j1Element* image;
	j1Element* entity_type_Image;
};

struct In_Game_Basics {
	j1Element* Image;
	j1Element* Resources;
};


struct Building_Menu {
	j1Element* entity_type_Image;
	j1Element* entity_name_boathouse;
	j1Element* Boat_Building_Button;
};

struct Boat_Menu {
	j1Element* entity_type_Image;
	j1Element* entity_name_boat;
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
	void Deactivate_All_UI();
	void Activate_Necessary_UI();
	void Activate_Menu();



	//BOAT_BUILDING
	void Activate_Building_Menu();
	void Deactivate_Building_Menu();

	//BOAT
	void Activate_Boat_Menu();
	void Deactivate_Boat_Menu();



	void Manage_Entity_UI(j1Entity* entity);


	void Activate_Manager();
	void Deactivate_Manager();


	void GUI_Event_Manager(GUI_Event type, j1Element* element);
	
	
	bool  clicking_ui = false;

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
	Unit_Manager_UI manager;
	In_Game_Basics basics;

	//ENtities
	Building_Menu building;
	Boat_Menu boat;

	j1Entity* selected = nullptr;
	int selected_offset;
	int selected_total;

	void GetSelectedEntity();

	
	int font_name = -1;
	bool quit = false;
};

#endif // __j1WINDOW_H__