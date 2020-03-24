#include "j1App.h"
#include "j1TransitionManager.h"
#include "j1Transitions.h"
#include "j1Render.h"
#include "j1Window.h"
#include "FadeToColor.h"
#include "MoveToMouse.h"
#include "Zoom.h"
#include "Wipe.h"
#include "Lines.h"
#include "Squares.h"
#include "j1Input.h"

j1TransitionManager::j1TransitionManager()
{}

j1TransitionManager::~j1TransitionManager()
{}

bool j1TransitionManager::PostUpdate() {
	
	// Calls PostUpdate (switch of states) of every transition in the list
	for (std::list<j1Transitions*>::iterator item = transitions_list.begin(); item != transitions_list.end(); ++item) {
		(*item)->PostUpdate();
	}
	return true;
}

bool j1TransitionManager::CleanUp() {
	
	// Clears the list
	transitions_list.clear();

	return true;
}

void j1TransitionManager::CleanTransitions(j1Transitions* transition) {
	
	// Removes the list and deletes the transition
	transitions_list.remove(transition);
	delete transition;
}

void j1TransitionManager::FadingToColor(j1Color color, float time) {
	transitions_list.push_back(new FadeToColor(color, time));
}

void j1TransitionManager::MovingToMouse(iPoint mouse_position, float time) {
	transitions_list.push_back(new MoveToMouse(mouse_position, time));
}

void j1TransitionManager::ZoomToMouse(iPoint mouse_position, bool repeat, float zoom, float time) {
	transitions_list.push_back(new Zoom(mouse_position, repeat, zoom, time));
}

void j1TransitionManager::Wiping(j1Color color, float time) {
	transitions_list.push_back(new Wipe(color, time));
}

void j1TransitionManager::LinesAppearing(j1Color color, float time) {
	transitions_list.push_back(new Lines(color, time));
}

void j1TransitionManager::SquaresAppearing(int transition, j1Color color, float time) {
	transitions_list.push_back(new Squares(transition, color, time));
}