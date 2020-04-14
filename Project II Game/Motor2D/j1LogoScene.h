#ifndef __j1LOGOSCENE_H__
#define __j1LOGOSCENE_H__

#include "j1Module.h"

struct SDL_Texture;


class j1LogoScene : public j1Module
{
public:

	j1LogoScene();

	// Destructor
	virtual ~j1LogoScene();

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

	void ChangeScene();

public:

	SDL_Texture* logo_texture = nullptr;

};

#endif // __j1LOGOSCENE_H__