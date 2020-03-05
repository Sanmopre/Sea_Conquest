#ifndef __J1GUITEXT__
#define __J1GUITEXT__

#include "j1Module.h"
#include"j1GUIelement.h"
#include "j1GUI.h"

struct SDL_Texture;

class j1GUItext : public j1GUIelement
{
public:

	j1GUItext();
	~j1GUItext();

	bool Awake(pugi::xml_node&);

	bool PreUpdate();
	bool PostUpdate();

	bool CleanUp();

	bool Load(pugi::xml_node&) { return true; };
	bool Save(pugi::xml_node&) const  const { return true; };

};


#endif // !__J1GUIIMAGE__


