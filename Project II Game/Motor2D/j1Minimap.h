#include "SDL\include\SDL.h"
#include "j1App.h"
#include "j1Module.h"
#include "p2Point.h"
#include "j1AnimationManager.h"
#include "j1Timer.h"

struct SDL_Texture;
struct SDL_Rect;
struct Chunk;
struct j1Entity;


class j1Minimap : public j1Module
{
public:
	j1Minimap();
	virtual ~j1Minimap();

	bool Awake(pugi::xml_node& config);

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void RenderMinimapTile(int x, int y, SDL_Rect r);
	fPoint MapToMinimap(int x, int y);
	iPoint MinimapToMap(float x, float y);

	void RenderMinimapFog(Chunk* chunk);

	bool display;
	iPoint position;
	fPoint map_position;

	bool clicking_map = false;

	void DrawCamera();
	void MinimapToWorldCamera();
	void Draw_entities(j1Entity* entity);

	SDL_Texture* minimap_tex = nullptr;
	SDL_Texture* noise_tex = nullptr;
	SDL_Texture* fog_chunk_tex = nullptr;
	SDL_Texture* fog_sub_chunk_tex = nullptr;
	SDL_Texture* fog_sub_sub_chunk_tex = nullptr;
	SDL_Texture* fog_tile_tex = nullptr;

	Animation map_anim;
	Animation fog_chunk_anim;
	Animation fog_sub_chunk_anim;
	Animation fog_sub_sub_chunk_anim;
	Animation fog_tile_anim;

	SDL_Texture* minimap_camera = nullptr;

	int tiles_width = 6;
	int tiles_height = 4;
	int collumns = 12;

	//UNITS in minimap
	SDL_Texture* metal= nullptr;
	SDL_Texture* wood = nullptr;
	SDL_Texture* cotton = nullptr;
	SDL_Texture* enemy = nullptr;
	SDL_Texture* ally = nullptr;

	SDL_Rect unit = { 0,0,2,2 };
	SDL_Rect map_rect;
	SDL_Rect tile_rect;
	SDL_Rect cameraminimap = {0,0,26,15};

};

