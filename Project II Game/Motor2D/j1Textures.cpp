#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1AssetManager.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

Texture::~Texture()
{
	name.clear();
	SDL_DestroyTexture(texture);
}

j1Textures::j1Textures() : j1Module()
{
	name.create("textures");
}

j1Textures::~j1Textures()
{}

bool j1Textures::Awake(pugi::xml_node& config)
{
	LOG("Init Image library");
	bool ret = true;

	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

bool j1Textures::Start()
{
	LOG("start textures");
	bool ret = true;

	pugi::xml_document file;
	pugi::xml_parse_result result = file.load_file("config.xml");

	pugi::xml_node texture = file.first_child().child("textures").child("texture");
	for (texture; texture != NULL; texture = texture.next_sibling("texture"))
	{
		string name = texture.attribute("name").as_string();
		int level = texture.attribute("level").as_int();
		int team = texture.attribute("team").as_int();
		string path = texture.attribute("source").as_string();

		Load(path.c_str(), name, level, team);
	}

	return ret;
}

bool j1Textures::CleanUp()
{
	LOG("Freeing textures and Image library");

	for(vector<Texture*>::iterator tex = textures.begin(); tex != textures.end(); tex++)
	{
		UnLoad((*tex)->texture);
		delete* tex;
	}

	textures.erase(textures.begin(), textures.end());
	textures.shrink_to_fit();
	IMG_Quit();
	return true;
}

SDL_Texture* const j1Textures::Load(const char* path, string name, int level, int team)
{
	SDL_Texture* texture = NULL;
	//SDL_Surface* surface = IMG_Load(path);
	SDL_Surface* surface = IMG_Load_RW(App->assets->Load(path), 1);

	if(surface == NULL)
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	else
	{
		texture = LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	if (texture != NULL)
		textures.push_back(new Texture(name, level, team, texture));

	return texture;
}

bool j1Textures::UnLoad(SDL_Texture* texture)
{
	for (vector<Texture*>::iterator tex = textures.begin(); tex != textures.end(); tex++)
	{
		if(texture == (*tex)->texture)
		{
			delete	*tex;
			textures.erase(tex);
			return true;
		}
	}

	return false;
}

SDL_Texture* const j1Textures::LoadSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(App->render->renderer, surface);

	if(texture == NULL)
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	
	return texture;
}

void j1Textures::GetSize(const SDL_Texture* texture, uint& width, uint& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*) &width, (int*) &height);
}

SDL_Texture* const j1Textures::GetTexture(string name, int level, int team)
{
	for (vector<Texture*>::iterator texture = textures.begin(); texture != textures.end(); texture++)
		if (name.compare((*texture)->name) == 0 && (*texture)->level == level && (*texture)->team == team)
			return (*texture)->texture;

	return nullptr;
}
