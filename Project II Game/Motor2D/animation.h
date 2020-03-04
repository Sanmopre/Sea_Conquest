#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"

//#include "j1App.h"
#define MAX_FRAMES 200
typedef unsigned int uint;
enum class Entity_Type;

class Animation
{
public:
	bool loop = true;
	float defspeed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
	float speeds[MAX_FRAMES];
	uint n_frames = 0;
	Entity_Type type;
	SDL_Texture* texture;

private:
	float current_frame;
	int current_speed;
	int last_frame = 0;
	int loops = 0;

public:

	SDL_Rect& GetCurrentFrame();

	void PushBack(const SDL_Rect& rect);
	bool Finished() const;
	void Reset();
	void SetFrame(int frame);
	int GetFrameNum();
	Animation GetAnimation();
};

#endif 