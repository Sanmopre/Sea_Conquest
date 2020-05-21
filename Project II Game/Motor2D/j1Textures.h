#ifndef __j1TEXTURES_H__
#define __j1TEXTURES_H__

#include "j1Module.h"

#include <vector>
#include <string>

using namespace std;

struct SDL_Texture;
struct SDL_Surface;

struct Texture
{
	Texture(string name, int level, int team, SDL_Texture* texture)
	{
		this->name = name;
		this->level = level;
		this->team = team;
		this->texture = texture;
	}
	~Texture();


	string name;
	int level;
	int team;
	SDL_Texture* texture;
};

class j1Textures : public j1Module
{
public:

	j1Textures();
	virtual ~j1Textures();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool CleanUp();

	SDL_Texture* const Load(const char* path, string name = "", int level = 0, int team = 0);
	bool UnLoad(SDL_Texture* texture);

	SDL_Texture* const GetTexture(string name, int level = 0, int team = 0);

	SDL_Texture* const LoadSurface(SDL_Surface* surface);
	void GetSize(const SDL_Texture* texture, uint& width, uint& height) const;

	vector<Texture*> textures;
};


#endif // __j1TEXTURES_H__