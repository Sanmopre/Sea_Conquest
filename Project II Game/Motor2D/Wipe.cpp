#include "j1Render.h"
#include "Wipe.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1App.h"
#include "j1Window.h"
#include "p2Log.h"

Wipe::Wipe(j1Color color, float time) : j1Transitions(time) {
	this->color = color;
	
	App->win->GetWindowSize(w, h);
	screen = { -(int)w, 0, (int)w, (int)h };
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
}

Wipe::~Wipe()
{}

void Wipe::Start() {

	j1Transitions::Start();

	screen.x = Interpolation(-(int)w, 0, percentage);

	SDL_SetRenderDrawColor(App->render->renderer, color.r, color.g, color.b, 255);
	SDL_RenderFillRect(App->render->renderer, &screen);	
}

void Wipe::Change() {

	j1Transitions::Change();

	SDL_SetRenderDrawColor(App->render->renderer, color.r, color.g, color.b, 255);
	SDL_RenderFillRect(App->render->renderer, &screen);

	if (App->scene->active)
		App->scene->ChangeScene();

	else
		App->scene2->ChangeScene();
}

void Wipe::Exit() {

	j1Transitions::Exit();

	screen.x = Interpolation(0, (int)w, percentage);

	SDL_SetRenderDrawColor(App->render->renderer, color.r, color.g, color.b, 255);
	SDL_RenderFillRect(App->render->renderer, &screen);
}