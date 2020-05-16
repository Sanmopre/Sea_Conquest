#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "j1Module.h"
#include "j1App.h"

#include <vector>
#include <string>

#define MAX_FRAMES 200

using namespace std;

class Animation
{
public:

	Animation() {}
	Animation(string name, float speed, bool loop)
	{
		this->name = name;
		this->speed = speed;
		this->loop = loop;
	}
	~Animation() 
	{
		name.clear();
	}

	string name;
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];

private:

	float current_frame;
	int last_frame = 0;
	int loops = 0;

public:

	SDL_Rect& GetCurrentFrame()
	{
		current_frame += speed*(App->dt);
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}

		return frames[(int)current_frame];
	}

	void PushBack(const SDL_Rect& rect) { frames[last_frame++] = rect; }

	bool Finished() const { return loops > 0; }

	void Reset() { current_frame = 0; }

	void SetFrame(int frame) { current_frame = frame; }
	int GetFrameNum() { return current_frame; }
};

class j1AnimationManager : public j1Module
{
public:

	j1AnimationManager();
	virtual ~j1AnimationManager();

	bool Start();
	bool CleanUp();

	Animation GetAnimation(string name);

private:

	void Animate(string name, int coll, int row, const int width, const int height, const int collumns, const int frames, float speed, bool loop);

	vector<Animation*> animations;
};

#endif