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
	j1Element* Trade;
};

struct Boat_Menu {
	j1Element* entity_type_Image;
	j1Element* entity_name_boat;
	j1Element* Trade;
};

struct Harvester_Menu {
	j1Element* entity_type_Image;
	j1Element* entity_name_Harvester;
	j1Element* Trade;
};

struct Trading_Menu_Selected {
	j1Element* Scroll;
	j1Element* Scroll_1;
	j1Element* Scroll_2;
	j1Element* back;
};

struct Trader_Menu {
	j1Element* image;
	j1Element* button_next;
	j1Element* buton_prev;
	j1Element* Scroll;
	j1Element* Scroll_1;
	j1Element* Scroll_2;
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


	//TRADING OPTIONS
	void Activate_Trading();
	void Deactivate_Trading();

	void Activate_Trader();
	void Deactivate_Trader();


	//BOAT_BUILDING
	void Activate_Building_Menu();
	void Deactivate_Building_Menu();

	//BOAT
	void Activate_Boat_Menu();
	void Deactivate_Boat_Menu();

	//HARVESTER
	void Activate_Harvester_Menu();
	void Deactivate_Harvester_Menu();

	void Update_Bar(j1Element* scroll, float resource, float total_resource);
	void Update_Resources(j1Entity* entity);

	void Manage_Entity_UI(j1Entity* entity);


	void Activate_Manager();
	void Deactivate_Manager();


	void GUI_Event_Manager(GUI_Event type, j1Element* element);
	
	
	bool  clicking_ui = false;

	j1Entity* selected = nullptr;

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
	Trading_Menu_Selected trading;
	Trader_Menu trader;
	Harvester_Menu harvester;

	//ENtities
	Building_Menu building;
	Boat_Menu boat;

	int selected_offset;
	int offset_modifier;
	int selected_total;

	void GetSelectedEntity();

	bool in_trading = false;
	int font_name = -1;
	bool quit = false;
};

#endif // __j1WINDOW_H__