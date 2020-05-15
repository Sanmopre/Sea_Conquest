#ifndef __J1PLAYER_H__
#define __J1PLAYER_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2Log.h"
#include "j1Entities.h"
#include "SDL/include/SDL_rect.h"

struct SDL_Texture;

class j1Player : public j1Module
{
public:
	j1Player();
	~j1Player();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	//Random number function with parameters
	int Randon_Number(int minimum_value, int maximum_value);

	//LOADING AND SAVING OPTIONS
	bool Save(pugi::xml_node& data);
	bool Load(pugi::xml_node& data);	

	bool disable_click;
	bool dragging = false;
	j1Entity* building;
	j1Entity* builder;
	
private:

	void Drag_Mouse();
	void Camera_Control(float dt);
	void Mouse_Cursor(float dt);
	void Select_Entitites(SDL_Rect);
	void Zoom();
	void Camera_Limit();

	SDL_Rect selector;
	SDL_Rect texture_rect{ 0,0,300,300 };

	int max_w_group;

	SDL_Texture* Tex_Player;
	SDL_Texture* highlight;
	Animation highlight_anim;

	iPoint mouse_position; 
	iPoint start_mouse_position;

	p2SString	folder;
	pugi::xml_node	node;
	p2SString file;

	uint win_height;
	uint win_width;
	int camera_speed;
	int camera_offset;
	SDL_Texture* UI_Image;

	bool lock_M1;
	bool lock_M2;
};

#endif // __j1PLAYER_H__