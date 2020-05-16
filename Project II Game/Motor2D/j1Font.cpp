#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Font.h"


#include "SDL\include\SDL.h"
#include "SDL_ttf\include\SDL_ttf.h"
#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib" )

j1Font::j1Font() : j1Module()
{
	name.create("fonts");
}

// Destructor
j1Font::~j1Font()
{}

// Called before render is available
bool j1Font::Awake(pugi::xml_node& conf)
{
	LOG("Init True Type Font library");
	bool ret = true;

	if (TTF_Init() == -1)
	{
		LOG("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		ret = false;
	}
	else
	{
		const char* path = conf.child("default_font").attribute("file").as_string(DEFAULT_FONT);
		int size = conf.child("default_font").attribute("size").as_int(DEFAULT_FONT_SIZE);
		default = Load(path, size);
	}

	return ret;
}

bool j1Font::Start()
{
	townhall = Print("TOWNHALL");
	boathouse = Print("BOATHOUSE");
	storage = Print("STORAGE");
	turret = Print("TURRET");

	boat = Print("BOAT");
	harvester = Print("HARVESTER");
	balloon = Print("BALLLOON");
	carrier = Print("CARRIER");

	windowed = Print("WINDOWED");
	fullscreen = Print("FULLSCREEN");
	explanation_on = Print("SHOW INFO");
	explanation_off = Print("HIDE INFO");

	return true;
}

bool j1Font::Update(float dt)
{
	return true;
}

// Called before quitting
bool j1Font::CleanUp()
{
	LOG("Freeing True Type fonts and library");
	p2List_item<TTF_Font*>* item;

	for (item = fonts.start; item != NULL; item = item->next)
	{
		TTF_CloseFont(item->data);
	}

	fonts.clear();
	TTF_Quit();
	return true;
}

// Load new texture from file path
TTF_Font* const j1Font::Load(const char* path, int size)
{
	TTF_Font* font = TTF_OpenFont(path, size);

	if (font == NULL)
	{
		LOG("Could not load TTF font with path: %s. TTF_OpenFont: %s", path, TTF_GetError());
	}
	else
	{
		LOG("Successfully loaded font %s size %d", path, size);
		fonts.add(font);
	}

	return font;
}

// Print text using font
SDL_Texture* j1Font::Print(const char* text, SDL_Color color, _TTF_Font* font)
{
	SDL_Texture* ret = NULL;
	SDL_Surface* surface = TTF_RenderText_Blended((font) ? font : default, text, color);
	if (text == "") {
		return ret;
	}
	else {
		if (surface == NULL)
		{
			LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		}
		else
		{
			ret = App->tex->LoadSurface(surface);
			SDL_FreeSurface(surface);
		}
	}
	return ret;
}

SDL_Texture* j1Font::Change_Name(names name)
{
	SDL_Texture* texture = nullptr;

	switch (name) {
	case names::TOWNHALL:
		texture = townhall;
		break;
	case names::STORAGE:
		texture = storage;
		break;
	case names::BOATHOUSE:
		texture = boathouse;
		break;
	case names::TURRET:
		texture = turret;
		break;
	case names::BOAT:
		texture = boat;
		break;
	case names::HARVESTER:
		texture = harvester;
		break;
	case names::BALLOON:
		texture = balloon;
		break;
	case names::CARRIER:
		texture = carrier;
		break;
	case names::WINDOWED:
		texture = windowed;
		break;
	case names::FULLSCREEN:
		texture = fullscreen;
		break;
	case names::EXPLANATIONS_OFF:
		texture = explanation_off;
		break;
	case names::EXPLANATIONS_ON:
		texture = explanation_on;
		break;
	}

	return texture;
}

// calculate size of a text
bool j1Font::CalcSize(const char* text, int& width, int& height, _TTF_Font* font) const
{
	bool ret = false;

	if (TTF_SizeText((font) ? font : default, text, & width, & height) != 0)
		LOG("Unable to calc size of text surface! SDL_ttf Error: %s\n", TTF_GetError());
	else
		ret = true;

	return ret;
}