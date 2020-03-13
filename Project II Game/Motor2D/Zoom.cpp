#include "j1Window.h"
#include "j1Render.h"
#include "Zoom.h"
#include "j1App.h"
#include "p2Log.h"
#include "j1Scene.h"
#include "j1Scene2.h"

Zoom::Zoom(iPoint mouse_position, bool repeat, float zoom, float time) : j1Transitions(time) {
	
	this->repeat = repeat;
	this->zoom = zoom;

	uint w, h;
	App->win->GetWindowSize(w, h);
	
	initial_pos.x = App->render->camera.x;
	initial_pos.y = App->render->camera.y;

	initial_zoom.x = App->render->camera.w;
	initial_zoom.y = App->render->camera.h;

	final_pos.x = (int)(-mouse_position.x + w / 2);
	final_pos.y = (int)(-mouse_position.y + h / 2);

	final_zoom.x = App->render->camera.w / 2;
	final_zoom.y = App->render->camera.h / 2;

	current_zoom.x = App->render->camera.w;
	current_zoom.y = App->render->camera.h;
}

Zoom::~Zoom()
{}

void Zoom::Start() {

	j1Transitions::Start();

	float scale = Interpolation(1.0f, zoom, percentage);

	fPoint zoom_screen;
	zoom_screen.x = Interpolation(initial_zoom.x, final_zoom.x, percentage);
	zoom_screen.y = Interpolation(initial_zoom.y, final_zoom.y, percentage);

	fPoint size;
	size.x = Interpolation(initial_pos.x, final_pos.x, percentage);
	size.y = Interpolation(initial_pos.y, final_pos.y, percentage);

	App->render->camera.x += zoom_screen.x - current_zoom.x;
	App->render->camera.y += zoom_screen.y - current_zoom.y;

	App->render->camera.x = size.x;
	App->render->camera.y = size.y;

	SDL_RenderSetScale(App->render->renderer, scale, scale);
	current_zoom = { (int)zoom_screen.x, (int)zoom_screen.y };
}

// If the change void is removed, the scene won't change
void Zoom::Change() {

	j1Transitions::Change();

	if (App->scene->active)
		App->scene->ChangeScene();

	else
		App->scene2->ChangeScene();
}
 
// If the exit void is removed, the scene will remain zoomed in
void Zoom::Exit() {

	j1Transitions::Exit();

	float scale = Interpolation(zoom, 1.0f, percentage);

	fPoint zoom_screen;
	zoom_screen.x = Interpolation(final_zoom.x, initial_zoom.x, percentage);
	zoom_screen.y = Interpolation(final_zoom.y, initial_zoom.y, percentage);

	fPoint size;
	size.x = Interpolation(final_pos.x, initial_pos.x, percentage);
	size.y = Interpolation(final_pos.y, initial_pos.y, percentage);

	App->render->camera.x -= zoom_screen.x - current_zoom.x;
	App->render->camera.y -= zoom_screen.y - current_zoom.y;

	App->render->camera.x = size.x;
	App->render->camera.y = size.y;

	SDL_RenderSetScale(App->render->renderer, scale, scale);
	current_zoom = { (int)zoom_screen.x, (int)zoom_screen.y };
}