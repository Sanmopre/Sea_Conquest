#include "j1Map.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Window.h"
#include "j1App.h"
#include "j1Pathfinding.h"
#include "j1Input.h"
#include "j1Fog.h"
#include "j1Minimap.h"
#include "j1EntityManager.h"
#include "j1Scene.h"

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

	dark_tiles = App->tex->GetTexture("dark-tiles", 0, 0);

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

	LoadEntities();
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
							if (prop == "WATER")
								terrain = NodeType::WATER;
							else if (prop == "GROUND")
								terrain = NodeType::GROUND;

							App->pathfinding->NodeMap.push_back(new Node(x, y, terrain));
						}
					}
}

void j1Map::LoadEntities()
{
	if (mapdata != nullptr)
	{
		App->scene->start = true;
		for (vector<Layer*>::iterator layer = mapdata->layers.begin(); layer != mapdata->layers.end(); layer++)
			if (GetLayerProperty((*layer)->id, "Entities").compare("true") == 0)
				for (int y = 0; y < (*layer)->layer_height; y++)
					for (int x = 0; x < (*layer)->layer_width; x++)
					{
						Tile* tile = GetTile((*layer)->layerdata[x][y]);
						if (tile != nullptr)
						{
							EntityRequest entity;

							fPoint pos = MapToWorld<fPoint>(x, y);
							entity.x = pos.x;
							entity.y = pos.y;

							string type = GetTileProperty(tile->id, "type");
							if (type == "townhall")
								entity.type = EntityType::TOWNHALL;
							else if (type == "boathouse")
								entity.type = EntityType::BOATHOUSE;
							else if (type == "storage")
								entity.type = EntityType::STORAGE;
							else if (type == "turret")
								entity.type = EntityType::TURRET;
							else if (type == "all cotton")
								entity.type = EntityType::ALL_COTTON;
							else if (type == "all wood")
								entity.type = EntityType::ALL_WOOD;
							else if (type == "all metal")
								entity.type = EntityType::ALL_METAL;
							else if (type == "boat")
								entity.type = EntityType::BOAT;

							string level = GetTileProperty(tile->id, "level");
							if (level == "1")
								entity.level = 1;
							else if (level == "2")
								entity.level = 2;
							else if (level == "3")
								entity.level = 3;

							string team = GetTileProperty(tile->id, "team");
							if (team == "0")
								entity.team = 0;
							else if (team == "1")
								entity.team = 1;
							else if (team == "2")
								entity.team = 2;

							App->entitymanager->AddEntity(entity.x, entity.y, entity.type, entity.level, entity.team);
						}
					}
		App->scene->start = false;
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

SDL_Rect j1Map::MapCulling(iPoint size, int extra_x, int extra_y)
{
	SDL_Rect ret;
	iPoint cam = { App->render->camera.x, App->render->camera.y };
	iPoint end = cam;
	cam.x /= -App->win->GetScale();
	cam.y /= -App->win->GetScale();
	cam = WorldToMap(cam.x, cam.y);
	cam.x -= extra_x * App->win->GetScale();
	cam.y -= extra_y * App->win->GetScale();

	end.x /= -App->win->GetScale();
	end.y /= -App->win->GetScale();
	end.x += App->render->camera.w;
	end.y += App->render->camera.h;
	end = WorldToMap(end.x, end.y);
	end.x += extra_x * App->win->GetScale();
	end.y += extra_y * App->win->GetScale();

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
				SDL_Rect cam = MapCulling({ (*layer)->layer_width, (*layer)->layer_height }, 10, 30);
				for (int y = cam.y; y < cam.h; y++)
					for (int x = cam.x; x < cam.w; x++)
					{
						bool visible = true;
						FogState visibility = App->fog->GetVisibility(x, y);
						if (App->ignore_fog)
							visibility = FogState::VISIBLE;

						if (visibility == FogState::FOGGED)
						{
							App->fog->RenderFogTile(x, y, 255);
							visible = false;
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

									SDL_Texture* texture = tile->tileset->texture;
									if (visibility == FogState::PARTIAL)
										texture = dark_tiles;
									App->render->AddBlitEvent(0, texture, (int)position.x - 32, (int)position.y - 32, rect);
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
	App->entitymanager->CleanUp();

	return true;
}

