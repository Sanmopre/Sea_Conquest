#include "j1Window.h"
#include "j1Render.h"
#include "MoveToMouse.h"
#include "j1App.h"

MoveToMouse::MoveToMouse(iPoint mouse_position, float time) : j1Transitions(time) {

	uint w, h;
	App->win->GetWindowSize(w, h);

	initial_pos.x = App->render->camera.x;
	initial_pos.y = App->render->camera.y;

	final_pos.x = (int)(-mouse_position.x + w / 2);
	final_pos.y = (int)(-mouse_position.y + h / 2);
}

MoveToMouse::~MoveToMouse()
{}

void MoveToMouse::Start() {

	j1Transitions::Start();

	fPoint size;

	size.x = Interpolation(initial_pos.x, final_pos.x, percentage);
	size.y = Interpolation(initial_pos.y, final_pos.y, percentage);

	App->render->camera.x = size.x;
	App->render->camera.y = size.y;
}