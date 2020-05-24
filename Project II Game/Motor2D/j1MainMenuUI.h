#ifndef __j1MAINMENUUI_H__
#define __j1MAINMENUUI_H__

#include "j1Module.h"
#include "SDL/include/SDL_rect.h"
#include <vector>

struct Main_Menu {

	j1Element* continue_button;
	j1Element* start;
	j1Element* quit;
	j1Element* audio_button;
	j1Element* fullscreen;
	j1Element* music_label;
	j1Element* music;
	j1Element* fx_label;
	j1Element* fx;
	j1Element* audio_image;
	j1Element* link;
	j1Element* instructions;
};


class j1MainMenuUI : public j1Module
{
public:
	j1MainMenuUI();
	~j1MainMenuUI();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	void Add_UI();
	void Activate_Menu();
	void Deactivate_Menu();
	void Activate_Audio_Options();
	void Deactivate_Audio_Options();
	void GUI_Event_Manager(GUI_Event type, j1Element* element);
	Main_Menu GetMenu();
	bool  clicking_ui = false;
	Main_Menu menu;
private:
	bool quit = true;
	bool audioopt = false;

};

#endif // __j1MAINMENUUI_H__