#ifndef __j1EXPLANATION_H__
#define __j1EXPLANATION_H__

#include "j1Module.h"
#include "p2Point.h"

struct Explanation_UI 
{
	j1Element* Image;
};

struct Explanation
{
	char* first;
	char* second;
	char* third;
	char* forth;
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


    void Show_Label(Text text = Text::NONE);

private:

	void Update_Position(j1Element* element = nullptr);
	bool Change_Label_Text(Explanation explanation);
	void Activate_Explanation();
	void Deactivate_Explanation();

public:
	j1Element* first = nullptr;
	j1Element* second = nullptr;
	j1Element* third = nullptr;
	j1Element* forth = nullptr;

	Explanation_UI explanation;

	Explanation boat;
	Explanation boathouse;
};

#endif //  __j1GUI_H__