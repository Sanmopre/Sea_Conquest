#ifndef __j1DIALOGSYSTEM_H__
#define __j1DIALOGSYSTEM_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Timer.h"

struct DialogSystem
{
	j1Element* Image;
	j1Element* Label;

};

enum class Dialog
{
	PIRATE,
	FUN,
	HAHA,
	ARROGANCE,
	NO,
	NONE
};

class j1DialogSystem : public j1Module
{

public:

	j1DialogSystem();
	~j1DialogSystem();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);

	void Display_Dialog(Dialog dialog = Dialog::NONE);
	void Hide_Dialogs();
public:
	iPoint position;
	
	
	DialogSystem pirate;
	DialogSystem pirate_fun; 
	DialogSystem haha;
	DialogSystem arrogance;
	DialogSystem no;



	bool show_dialog = false;

	timed_var timer;
	bool in_timer = false;
	int Display_timer = 0;
};

#endif //  __j1DIALOGSYSTEM_H__