#pragma once

#include "j1TransitionManager.h"
#include "j1Transitions.h"

class Wipe : public j1Transitions {

public:

	Wipe(j1Color color, float time);
	~Wipe();

	void Start();
	void Change();
	void Exit();

private:

	uint w, h;

	SDL_Rect screen;
	j1Color color;

	float initial_width;
	float final_width;
	float width;
	
};