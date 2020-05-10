#include "j1Map.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Window.h"
#include "j1App.h"
#include "j1Pathfinding.h"
#include "j1Input.h"
#include "j1Fog.h"

TileSet::~TileSet()
{
	name.clear();

	App->tex->UnLoad(texture);
}

j1Map::j1Map()
{
}

j1Map::~j1Map()
{
	CleanUp();
}

void j1Map::LoadMap(const char* path)
{
	CleanUp();

	pugi::xml_parse_result result = map_data.load_file(path);

	pugi::xml_node node = map_data.first_child();

	string o = node.attribute("orientation").as_string();

	MapOrientation orientation = MapOrientation::NONE;
	if(o.compare("isometric") == 0)
		orientation = MapOrientation::ISOMETRIC;
	else if (o.compare("orthogonal") == 0)
		orientation = MapOrientation::ORTHOGONAL;

	int width = node.attribute("width").as_int();
	int height = node.attribute("height").as_int();
	int tilewidth = node.attribute("tilewidth").as_int();
	int tileheight = node.attribute("tileheight").as_int();

	mapdata = new MapData(orientation, width, height, tilewidth, tileheight);

	node = node.first_child();

	for (node; node != NULL; node = node.next_sibling("tileset"))
	{
		string name = node.attribute("name").as_string();
		int firstgid = node.attribute("firstgid").as_int();
		int tileswidth = node.attribute("tilewidth").as_int();
		int tilesheight = node.attribute("tileheight").as_int();
		int columns = node.attribute("columns").as_int();
		string path = node.child("image").attribute("source").as_string(); //SDL_Texture
		SDL_Texture* texture = App->tex->Load(path.c_str(), name);

		TileSet *tileset = new TileSet(name, firstgid, tileswidth, tilesheight, columns, texture);

		mapdata->tilesets.push_back(tileset);

		LoadTiles(node, *(mapdata->tilesets.end() - 1));
	}

	node = map_data.first_child();
	LoadLayers(node);

	LoadPathNodes();

	if(App->input->GetKey(SDL_SCANCODE_F1) != KEY_REPEAT)
		App->pathfinding->LoadIslands();
}

void j1Map::LoadTiles(pugi::xml_node& node, TileSet* tileset)
{
	pugi::xml_node tile_node = node.child("tile");

	for (tile_node; tile_node != NULL; tile_node = tile_node.next_sibling("tile"))
	{
		int id = tile_node.attribute("id").as_int() + tileset->firstgid;

		Tile* tile = new Tile(id);
		tile->tileset = tileset;

		LoadProperties(tile_node, tile->properties);

		if (id != 1)
		{
			int n = id - (*(mapdata->tiles.end() - 1))->id;
			if (n != 1)
			{
				for (int i = n - 1; i >= 1; i--)
				{
					mapdata->tiles.push_back(new Tile(id - i));
				}
			}
		}
		mapdata->tiles.push_back(tile);
	}
}

void j1Map::LoadLayers(pugi::xml_node& node)
{
	node = map_data.first_child().child("layer");

	for (node; node != NULL; node = node.next_sibling("layer"))
	{
		string name = node.attribute("name").as_string();
		int id = node.attribute("id").as_int();
		int layer_width = node.attribute("width").as_int();
		int layer_height = node.attribute("height").as_int();

		Layer *layer = new Layer(name, id, layer_width, layer_height);

		LoadProperties(node, layer->properties);

		layer->layerdata = new int* [layer_width];
		for (int i = 0; i < layer_width; ++i)
			layer->layerdata[i] = new int[layer_height];

		pugi::xml_node tile_node = node.child("data").child("tile");

		int x = 0;
		int y = 0;
		for (tile_node; tile_node != NULL; tile_node = tile_node.next_sibling("tile"))
		{
			layer->layerdata[x][y] = tile_node.attribute("gid").as_uint();
			x++;
			if (x == layer_width)
			{
				x = 0;
				y++;
			}
		}

		mapdata->layers.push_back(layer);

		if (GetLayerProperty(layer->id, "ToDraw").compare("true") == 0)
		{
			App->fog->LoadMap(layer_width, layer_height);
		}
	}
}

void j1Map::LoadProperties(pugi::xml_node& node, vector<Property>& properties)
{
	pugi::xml_node prop_node = node.child("properties").first_child();

	for (prop_node; prop_node != NULL; prop_node = prop_node.next_sibling("property"))
	{
		string name = prop_node.attribute("name").as_string();
		string data = prop_node.attribute("value").as_string();

		Property property = Property(name, data);

		properties.push_back(property);
	}
}

void j1Map::LoadPathNodes()
{
	if (mapdata != nullptr)
		for (vector<Layer*>::iterator layer = mapdata->layers.begin(); layer != mapdata->layers.end(); layer++)
			if (GetLayerProperty((*layer)->id, "TerrainNodes").compare("true") == 0)
				for (int y = 0; y < (*layer)->layer_height; y++)
					for (int x = 0; x < (*layer)->layer_width; x++)
					{
						Tile* tile = GetTile((*layer)->layerdata[x][y]);
						if (tile != nullptr)
						{
							NodeType terrain = NodeType::WATER;
							string prop = GetTileProperty(tile->id, "terrain");
							if (prop.compare("WATER") == 0)
								terrain = NodeType::WATER;
							else if (prop.compare("GROUND") == 0)
								terrain = NodeType::GROUND;

							App->pathfinding->NodeMap.push_back(new Node(x, y, terrain));
						}
					}
}

string j1Map::GetTileProperty(int id, string name)
{
	string ret;

	if (mapdata->tiles.size() >= id && id > 0)
	{
		vector<Tile*>::iterator tile = mapdata->tiles.begin() + id - 1;

		for (vector<Property>::iterator prop = (*tile)->properties.begin(); prop != (*tile)->properties.end(); prop++)
		{
			if (prop->name.compare(name) == 0 || name.compare("") == 0)
			{
				ret = prop->data;
				break;
			}
		}
	}

	return ret;
}

string j1Map::GetLayerProperty(int id, string name)
{
	string ret;

	if (mapdata->layers.size() >= id && id > 0)
	{
		vector<Layer*>::iterator layer = mapdata->layers.begin() + id - 1;

		for (vector<Property>::iterator prop = (*layer)->properties.begin(); prop != (*layer)->properties.end(); prop++)
		{
			if (prop->name.compare(name) == 0 || name.compare("") == 0)
			{
				ret = prop->data;
				break;
			}
		}
	}

	return ret;
}

SDL_Rect j1Map::MapCulling(iPoint size)
{
	SDL_Rect ret;
	iPoint cam = { App->render->camera.x, App->render->camera.y };
	iPoint end = cam;
	cam.x /= -App->win->GetScale();
	cam.y /= -App->win->GetScale();
	cam = WorldToMap(cam.x, cam.y);
	cam.x -= 10 * App->win->GetScale();
	cam.y -= 30 * App->win->GetScale();

	end.x /= -App->win->GetScale();
	end.y /= -App->win->GetScale();
	end.x += App->render->camera.w;
	end.y += App->render->camera.h;
	end = WorldToMap(end.x, end.y);
	end.x += 1 * App->win->GetScale();
	end.y += 30 * App->win->GetScale();

	if (cam.x < 0)
		cam.x = 0;
	else if (cam.x > size.x)
		cam.x = size.x;
	if (cam.y < 0)
		cam.y = 0;
	else if (cam.y > size.y)
		cam.y = size.y;
	if (end.x < 0)
		end.x = 0;
	else if (end.x > size.x)
		end.x = size.x;
	if (end.y < 0)
		end.y = 0;
	else if (end.y > size.y)
		end.y = size.y;

	ret = { cam.x, cam.y, end.x, end.y };

	return ret;
}

void j1Map::Draw()
{
	if (mapdata != nullptr)
	{
		for (vector<Layer*>::iterator layer = mapdata->layers.begin(); layer != mapdata->layers.end(); layer++)
		{	
			if (GetLayerProperty((*layer)->id, "ToDraw").compare("true") == 0)
			{
				SDL_Rect cam = MapCulling({ (*layer)->layer_width, (*layer)->layer_height });
				for (int y = cam.y; y < cam.h; y++)
					for (int x = cam.x; x < cam.w; x++)
					{
						bool visible = true;
						FogState visibility = App->fog->GetVisibility(x, y);
						if (App->godmode)
							visibility = FogState::VISIBLE;
						switch (visibility)
						{
						case FogState::FOGGED:
							App->fog->RenderFogTile(x, y, 255);
							visible = false;
							break;
						case FogState::PARTIAL:
							App->fog->RenderFogTile(x, y, 100);
							break;
						case FogState::VISIBLE:
							break;
						}
						if (visible)
						{
							if ((*layer)->layerdata[x][y] != 0)
							{
								Tile* tile = GetTile((*layer)->layerdata[x][y]);
								if (tile != nullptr)
								{
									fPoint position = MapToWorld<fPoint>(x, y);

									int relative_id = tile->id - tile->tileset->firstgid;
									SDL_Rect rect;
									rect.w = tile->tileset->tileswidth;
									rect.h = tile->tileset->tilesheight;
									int t = relative_id / tile->tileset->columns;
									int n = relative_id - (t * tile->tileset->columns);
									rect.x = rect.w * n;
									rect.y = rect.h * t;

									App->render->AddBlitEvent(0, tile->tileset->texture, (int)position.x - 32, (int)position.y - 32, rect);
								}
							}
						}
					}
			}
		}
	}
}

Tile* j1Map::GetTile(int id)
{
	if (id <= 0)
		return nullptr;
	else if (id > mapdata->tiles.size())
		return nullptr;
	else
	return *(mapdata->tiles.begin() + id - 1);

	return nullptr;
}

iPoint j1Map::WorldToMap(float x, float y) const
{
	iPoint ret(0, 0);

	if (mapdata->orientation == MapOrientation::ORTHOGONAL)
	{
		ret.x = x / mapdata->tilewidth;
		ret.y = y / mapdata->tileheight;
	}
	else if (mapdata->orientation == MapOrientation::ISOMETRIC)
	{
		float half_width = mapdata->tilewidth * 0.5f;
		float half_height = mapdata->tileheight * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2);
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}

	return ret;
}

bool j1Map::CleanUp()
{
	map_data.reset();
	if (mapdata != nullptr)
	{
		delete mapdata;
		mapdata = nullptr;
	}
	App->pathfinding->CleanUp();
	App->fog->CleanUp();

	return true;
}

