#ifndef _j1TRANSITIONMANAGER_H__
#define _j1TRANSITIONMANAGER_H__

#include "j1Module.h"
#include "j1Color.h"
#include "j1Transitions.h"
#include <list>

class j1TransitionManager : public j1Module {
public:

	j1TransitionManager();
	~j1TransitionManager();

	virtual bool Awake(pugi::xml_node&) { return true; }
	virtual bool Start() { return true; }
	virtual bool PostUpdate();
	virtual bool CleanUp();

	void FadingToColor(j1Color color, float time, int scene);
	void Wiping(j1Color color, float time, int scene);
	void LinesAppearing(j1Color color, float time, int scene);
	void SquaresAppearing(int transition, j1Color color, float time, int scene);

	void CleanTransitions(j1Transitions* ended_transition);

public:

	bool transition;

private:

	std::list<j1Transitions*> transitions_list;

};

#endif