#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1GUIelement.h"
#include <vector>


class j1Entity;


class j1GUI : public j1Module
{

public:

	j1GUI();
	~j1GUI();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);

	void UpdatePosition(j1GUIelement* element, iPoint position, iPoint localPosition);
	j1GUIelement* AddElement(GUItype type, j1GUIelement* parent, iPoint Map_Position, iPoint Inside_Position, bool interactable, bool enabled, SDL_Rect section, char* text = nullptr, j1Module* listener = nullptr, bool X_drag = false, bool Y_drag = false, SCROLL_TYPE scrollType = SCROLL_TYPE::SCROLL_NONE, bool decor = false);
	p2List<j1GUIelement*>	GUI_ELEMENTS;
	//std::vector<j1GUIelement*> gui_elements;
};

#endif //  __j1GUI_H__

