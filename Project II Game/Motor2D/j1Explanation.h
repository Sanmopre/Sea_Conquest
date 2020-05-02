#ifndef __j1EXPLANATION_H__
#define __j1EXPLANATION_H__

#include "j1Module.h"
#include "p2Point.h"

class j1Explanation : public j1Module
{

public:

	j1Explanation();
	~j1Explanation();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();


public:

};

#endif //  __j1GUI_H__