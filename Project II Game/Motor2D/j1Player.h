#ifndef __J1PLAYER_H__
#define __J1PLAYER_H__

#include "j1Module.h"
#include "j1Entities.h"
#include "p2Point.h"
#include "p2Log.h"
#include "SDL/include/SDL_rect.h"

struct SDL_Texture;

enum states {
	DRAGGING,
	RELEASE
};


class j1Player : public j1Module
{
public:
	j1Player();
	~j1Player();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update();
	bool CleanUp();

	//LOADING AND SAVING OPTIONS
	bool Save(pugi::xml_node& data);
	bool Load(pugi::xml_node& data);	
	

	void Drag_Mouse();
	void Camera_Control();
private:


	SDL_Rect selector;

	iPoint mouse_position; 
	iPoint start_mouse_position;
	SDL_Texture* Tex_Player;
	states state;	

	p2SString	folder;
	pugi::xml_node	node;
	p2SString file;


	int camera_speed = 1;
};

#endif // __j1PLAYER_H__