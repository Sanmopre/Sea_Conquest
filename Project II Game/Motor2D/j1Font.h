#ifndef __j1FONT_H__
#define __j1FONT_H__

#include "j1Module.h"
#include "p2List.h"
#include "SDL\include\SDL_pixels.h"

#define DEFAULT_FONT "fonts/open_sans/Head.ttf"
#define DEFAULT_FONT_SIZE 20

struct SDL_Texture;
struct _TTF_Font;

enum  names {
	//BUILDINGS
	BOATHOUSE,
	TOWNHALL,
	STORAGE,

	//UNITS
	BOAT,
	HARVESTER,
	BALLOON,
	CARRIER,

    //UI BUTTONS
	WINDOWED,
	FULLSCREEN,
	EXPLANATIONS_ON,
	EXPLANATIONS_OFF,

	NO_NAME
};

class j1Font : public j1Module
{
public:

	j1Font();

	// Destructor
	virtual ~j1Font();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	bool Start();
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Load Font
	_TTF_Font* const Load(const char* path, int size = 12);

	// Create a surface from text
	SDL_Texture* Print(const char* text, SDL_Color color = { 255, 255, 255, 255 }, _TTF_Font* font = NULL);

	SDL_Texture* Change_Name(names name = names::NO_NAME);

	bool CalcSize(const char* text, int& width, int& height, _TTF_Font* font = NULL) const;

public:

	p2List<_TTF_Font*>	fonts;
	_TTF_Font* default;

private:
	//buildings
	SDL_Texture* townhall;
	SDL_Texture* boathouse;
	SDL_Texture* storage;


	//units
	SDL_Texture* boat;
	SDL_Texture* harvester;
	SDL_Texture* balloon;
	SDL_Texture* carrier;

	//ui
	SDL_Texture* windowed;
	SDL_Texture* fullscreen;
	SDL_Texture* explanation_on;
	SDL_Texture* explanation_off;
};


#endif // __j1FONT_H__