#ifndef __j1EXPLANATION_H__
#define __j1EXPLANATION_H__

#include "j1Module.h"
#include "p2Point.h"

struct Explanation_UI 
{
	j1Element* Image;
};

struct Text_Label
{
	j1Element* Boat;
	j1Element* Boathouse;
	j1Element* Storage;
	j1Element* Harvester;
};

enum class Text
{
	BOAT,
	BOATHOUSE,
	STORAGE,
	HARVESTER,
	NONE
};

class j1Explanation : public j1Module
{

public:

	j1Explanation();
	~j1Explanation();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();

	void Show_Information(Text text = Text::NONE);
	void Update_Position(j1Element* element = nullptr);
	j1Element* Change_Label( Text text = Text::NONE);

public:

	Text_Label label;
	Explanation_UI explanation;
};

#endif //  __j1GUI_H__