#ifndef __j1INGAMEUI_H__
#define __j1INGAMEUI_H__

#include "j1Module.h"
#include "SDL/include/SDL_rect.h"

struct Game_Menu {

	j1GUIelement* Menu_button = nullptr;
	j1GUIelement* Title = nullptr;
	j1GUIelement* Image = nullptr;
	j1GUIelement* Exit_button = nullptr;
	j1GUIelement* Return_button = nullptr;
	j1GUIelement* Resume_button = nullptr;
	j1GUIelement* Music_scroll = nullptr;
	j1GUIelement* Save = nullptr;
	j1GUIelement* Load = nullptr;
	j1GUIelement* label1 = nullptr;

};

struct SDL_Texture;

class j1InGameUI : public j1Module
{
public:

	j1InGameUI();
	~j1InGameUI();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	void Add_UI();
	void Activate_Menu();
	void GUI_Event_Manager(GUI_Event type, j1GUIelement* element);

private:
	//positions
	int MiddleScreenW;
	int MiddleScreenH;
	int width;
	

	//resources
	int type_0;
	int type_1;
	int type_2;

	char text_type_0[10];
	char text_type_1[10];
	char text_type_2[10];


	//menu ui
	Game_Menu menu;
	SDL_Texture* resources;
	SDL_Texture* UI_Image;
	SDL_Rect texture_rect{ 0,0,1280,200 };
	SDL_Rect texture_rect_1{ 0,0,400,30 };
	int font_name = -1;
};

#endif // __j1WINDOW_H__