#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;


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

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();




	void Add_UI();
	void Activate_Menu();
	void GUI_Event_Manager(GUI_Event type, j1GUIelement* element);

private:
	SDL_Texture* debug_tex;
	j1GUIelement* Menu_button = nullptr;

	Game_Menu menu;
};

#endif // __j1SCENE_H__