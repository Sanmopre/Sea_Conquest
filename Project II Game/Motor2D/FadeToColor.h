#pragma once

#include "j1TransitionManager.h"
#include "j1Transitions.h"

class FadeToColor : public j1Transitions {
public:

	FadeToColor(j1Color color, float time, int scene);
	~FadeToColor();

	void Start();
	void Change();
	void Exit();
	
	void FillScreen(float percentage);
	
private:

	j1Color color;
	SDL_Rect screen;

	bool changing_scene = false;

	int scene;
};