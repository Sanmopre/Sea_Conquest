#pragma once

#include "j1Transitions.h"
#include "j1TransitionManager.h"

class MoveToMouse : public j1Transitions {

public:

	MoveToMouse(iPoint mouse_position, float time);
	~MoveToMouse();

	void Start();

private:

	iPoint initial_pos;
	iPoint final_pos;
};