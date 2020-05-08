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
	QUEST,
	TRADE,
	SELECT_QUEST,
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
	
	bool information_mode = false;

private:

	void Update_Position(j1Element* element = nullptr , int pos_x = 0, int pos_y = 0);
	bool Change_Label_Text(Explanation explanation);
	void Activate_Explanation();
	void Deactivate_Explanation();
	bool explanation_active = false;
	j1Element* first = nullptr;
	j1Element* second = nullptr;
	j1Element* third = nullptr;
	j1Element* forth = nullptr;

	j1Element* information_image;
	Explanation_UI explanation;

	Explanation boat;
	Explanation boathouse;
	Explanation storage;
	Explanation harvester;
	Explanation quest;
	Explanation trader;
	Explanation select_quest;



};

#endif //  __j1GUI_H__