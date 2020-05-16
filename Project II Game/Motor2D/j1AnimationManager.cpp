#include "j1AnimationManager.h"

j1AnimationManager::j1AnimationManager() {}

j1AnimationManager::~j1AnimationManager()
{
	while (animations.size() != 0)
	{
		delete* animations.begin();
		animations.erase(animations.begin());
	}
	animations.shrink_to_fit();
}

bool j1AnimationManager::Start()
{
	pugi::xml_document file;
	pugi::xml_parse_result result = file.load_file("config.xml");

	pugi::xml_node animation = file.first_child().child("animations").child("animation");
	for (animation; animation != NULL; animation = animation.next_sibling("animation"))
	{
		string name = animation.attribute("name").as_string();
		int coll = animation.attribute("coll").as_int();
		int row = animation.attribute("row").as_int();
		int width = animation.attribute("width").as_int();
		int height = animation.attribute("height").as_int();
		int collumns = animation.attribute("collumns").as_int();
		int frames = animation.attribute("frames").as_int();
		float speed = animation.attribute("speed").as_float();
		bool loop = animation.attribute("loop").as_bool();

		Animate(name, coll, row, width, height, collumns, frames, speed, loop);
	}

	return true;
}

bool j1AnimationManager::CleanUp()
{
	while (animations.size() != 0)
	{
		delete* animations.begin();
		animations.erase(animations.begin(), animations.end());
	}

	vector<Animation*> a;
	animations.swap(a);

	return true;
}

void j1AnimationManager::Animate(string name, int coll, int row, const int width, const int height, const int collumns, const int frames, float speed, bool loop)
{
	Animation* anim = new Animation(name, speed, loop);
	for (int i = 0; i < frames; i++)
	{
		anim->PushBack({ width * coll, height * row, width, height });
		coll++;
		if (coll == collumns)
		{
			coll = 0;
			row++;
		}
	}

	animations.push_back(anim);
}

Animation j1AnimationManager::GetAnimation(string name)
{
	for (vector<Animation*>::iterator animation = animations.begin(); animation != animations.end(); animation++)
		if (name == (*animation)->name)
			return **animation;
		
	return Animation();
}