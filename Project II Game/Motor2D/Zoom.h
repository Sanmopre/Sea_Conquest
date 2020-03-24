#pragma once

#include "j1TransitionManager.h"
#include "j1Transitions.h"

class Zoom : public j1Transitions {

public:

	Zoom(iPoint mouse_position, bool repeat, float zoom, float speed);
	~Zoom();

	void Start();
	void Change();
	void Exit();

private:

	bool repeat = false;

	float zoom;
	iPoint initial_pos;
	iPoint initial_zoom;
	iPoint final_pos;
	iPoint final_zoom;

	iPoint current_zoom;
};