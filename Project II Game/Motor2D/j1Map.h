#ifndef _MAP_H_
#define _MAP_H_

#include "j1Module.h"
#include "p2Point.h"

#include <string>
#include <vector>

using namespace std;

struct SDL_Texture;
struct SDL_Rect;

enum class MapOrientation
{
	ISOMETRIC = 1,
	ORTHOGONAL = 2,
	NONE = 0
};

struct Property
{
	Property(string name, string data)
	{
		this->name = name;
		this->data = data;
	}
	~Property()
	{
		name.clear();
		data.clear();
	}

	string name;
	string data;
};

struct TileSet
{
	TileSet(string name, int firstgid, int tileswidth, int tilesheight, int columns, SDL_Texture* texture)
	{
		this->name = name;
		this->firstgid = firstgid;
		this->tileswidth = tileswidth;
		this->tilesheight = tilesheight;
		this->columns = columns;
		this->texture = texture;
	}
	~TileSet();

	string name;
	int firstgid;
	int tileswidth;
	int tilesheight;
	int columns;
	SDL_Texture* texture; //SDL_Texture
};

struct Tile
{
	Tile(int id)
	{ 
		this->id = id;
		tileset = nullptr;
	}
	~Tile()
	{
		vector<Property> v;
		properties.swap(v);
	}

	int id;
	TileSet* tileset;

	vector<Property> properties;
};

struct Layer
{
	Layer(string name, int id, int layer_width, int layer_height)
	{
		this->name = name;
		this->id = id;
		this->layer_width = layer_width;
		this->layer_height = layer_height;
	}
	~Layer()
	{
		name.clear();

		for (int i = 0; i < layer_width; ++i)
			delete[] layerdata[i];
		delete[] layerdata;

		vector<Property> v;
		properties.swap(v);
	}

	string name;
	int id;
	int layer_width;
	int layer_height;

	int** layerdata;
	vector<Property> properties;
};

struct MapData
{
	MapData(MapOrientation orientation, int width, int height, int tilewidth, int tileheight)
	{
		this->orientation = orientation;
		this->width = width;
		this->height = height;
		this->tilewidth = tilewidth;
		this->tileheight = tileheight;
	}
	~MapData()
	{
		while (tiles.size() != 0)
		{
			delete* tiles.begin();
			tiles.erase(tiles.begin());
		}
		tiles.shrink_to_fit();
		while (tilesets.size() != 0)
		{
			delete* tilesets.begin();
			tilesets.erase(tilesets.begin());
		}
		tilesets.shrink_to_fit();
		while (layers.size() != 0)
		{
			delete* layers.begin();
			layers.erase(layers.begin());
		}
		layers.shrink_to_fit();
	}

	MapOrientation orientation;
	int width;
	int height;
	int tilewidth;
	int tileheight;

	vector<TileSet*> tilesets;
	vector<Tile*> tiles;
	vector<Layer*> layers;
};

class j1Map : public j1Module
{
public:

	j1Map();
	virtual ~j1Map();

	bool CleanUp();

	void LoadMap(const char* path);
	void Draw();

	template<class type>
	type MapToWorld(int x, int y) const
	{
		type ret;

		if (mapdata->orientation == MapOrientation::ORTHOGONAL)
		{
			ret.x = x * mapdata->tilewidth;
			ret.y = y * mapdata->tileheight;
		}
		else if (mapdata->orientation == MapOrientation::ISOMETRIC)
		{
			ret.x = (x - y) * (mapdata->tilewidth * 0.5f);
			ret.y = (x + y) * (mapdata->tileheight * 0.5f);
		}

		return ret;
	}
	iPoint WorldToMap(float x, float y) const;

	string GetTileProperty(int id, string name = "");
	string GetLayerProperty(int id, string name = "");
	Tile* GetTile(int id);

	SDL_Rect MapCulling(iPoint size, int extra_x, int extra_y);

	MapData* mapdata = nullptr;

private:

	void LoadTiles(pugi::xml_node& node, TileSet* tileset);
	void LoadLayers(pugi::xml_node& node);
	void LoadProperties(pugi::xml_node& node, vector<Property>& properties);
	void LoadPathNodes();

	pugi::xml_document map_data;

	SDL_Texture* dark_tiles;
};

#endif //_MAP_H_