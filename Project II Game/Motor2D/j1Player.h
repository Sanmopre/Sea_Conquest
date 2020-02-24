#ifndef __J1PLAYER_H__
#define __J1PLAYER_H__

#include "j1Module.h"
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

public:


	SDL_Rect prueba{ 0,0,0,0 };



	iPoint mouse_position; 
	iPoint mouse_position2;
	SDL_Texture* Tex_Player;
	states state;	

	p2SString	folder;
	pugi::xml_node	node;
	p2SString file;
};

#endif // __j1PLAYER_H__