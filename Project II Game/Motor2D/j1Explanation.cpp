#include "j1App.h"
#include "j1GUIElements.h"

#include "j1Input.h"
#include "j1GUI.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Player.h"
#include "j1Explanation.h"

j1Explanation::j1Explanation()
{
}

j1Explanation::~j1Explanation()
{
}

bool j1Explanation::Awake(pugi::xml_node& config)
{
	return true;
}

bool j1Explanation::Start()
{
	return true;
}

bool j1Explanation::Update(float dt)
{
	return true;
}

bool j1Explanation::PostUpdate()
{
	return true;
}
