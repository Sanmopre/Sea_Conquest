#ifndef __j1FONT_H__
#define __j1FONT_H__

#include "j1Module.h"
#include "p2List.h"
#include "SDL\include\SDL_pixels.h"

#define DEFAULT_FONT "fonts/open_sans/zorque.ttf"
#define DEFAULT_FONT_SIZE 20

struct SDL_Texture;
struct _TTF_Font;

class j1Font : public j1Module
{
public:

	j1Font();

	// Destructor
	virtual ~j1Font();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Load Font
	_TTF_Font* const Load(const char* path, int size = 12);

	// Create a surface from text
	SDL_Texture* Print(const char* text, SDL_Color color = { 255, 255, 255, 255 }, _TTF_Font* font = NULL);

	bool CalcSize(const char* text, int& width, int& height, _TTF_Font* font = NULL) const;

public:

	p2List<_TTF_Font*>	fonts;
	_TTF_Font* default;
};


#endif // __j1FONT_H__