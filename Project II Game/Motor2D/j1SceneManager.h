#ifndef __j1SCENEMANAGER_H__
#define __j1SCENEMANAGER_H__

#include "j1Module.h"

struct SDL_Texture;
class j1Transitions;

class j1SceneManager : public j1Module
{
public:

	j1SceneManager();

	// Destructor
	virtual ~j1SceneManager();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	int ChangeScene(int scene);


public:
	int current_scene = 0;
	bool In_Main_Menu = false;

private: 

	j1Transitions* transition = nullptr;

};

#endif // __j1SCENEMANAGER_H__
