#include "j1App.h"
#include "j1Pathfinding.h"

#include "j1Map.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Input.h"
#include "Color.h"

bool j1PathFinding::Start()
{
	show = false;
	return true;
}

bool j1PathFinding::Update(float dt)
{
	if (App->godmode && App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		show = !show;

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		show = false;

	if (show)
	{
		for (auto i = islands.begin(); i != islands.end(); i++)
			for (auto itr = (*i)->map.begin(); itr != (*i)->map.end(); itr++)
			{
				iPoint ps = App->map->MapToWorld<iPoint>((*itr)->tile.x, (*itr)->tile.y);
				App->render->AddBlitEvent(2, nullptr, 0, 0, { ps.x-2,ps.y-2, 9,9 }, false, false, 0, 0, 255);
			}
		for (auto i = NodeMap.begin(); i != NodeMap.end(); i++)
		{
			fPoint point = App->map->MapToWorld<fPoint>((*i)->tile.x, (*i)->tile.y);
			Color c = {};
			if ((*i)->type == NodeType::GROUND)
				c.Red();
			else if ((*i)->type == NodeType::WATER)
				c.Green();
			if ((*i)->built)
				c.Black();
			App->render->AddBlitEvent(2, nullptr, 0, 0, { (int)point.x, (int)point.y, 5, 5 }, false, false, c.r, c.g, c.b, 255);
		}
	}

	return true;
}

void j1PathFinding::LoadIslands()
{
	for (vector<Node*>::iterator node = NodeMap.begin(); node != NodeMap.end(); node++)
	{
		if ((*node)->island == nullptr && (*node)->type == NodeType::GROUND)
			CreateIsland((*node)->tile, (*node)->type);
	}
}

void j1PathFinding::CreateIsland(iPoint position, NodeType terrain)
{
	Island* island = new Island();

	island->map = GetWalkableMap(position, terrain, island);

	islands.push_back(island);
}

vector<Node*> j1PathFinding::GetWalkableMap(iPoint position, NodeType terrain, Island* island)
{
	vector<Node*> ret;

	Expand(position, ret, terrain, island);

	return ret;
}

void j1PathFinding::Expand(iPoint node, vector<Node*>& map, NodeType terrain, Island* island)
{
	for (int y = -1; y <= 1; y++)
		for (int x = -1; x <= 1; x++)
			if (x == 0 && y == 0)
				continue;
			else if ((node.x + x) >= 0 && (node.x + x) < App->map->mapdata->width && (node.y + y) >= 0 && (node.y + y) < App->map->mapdata->height)
			{
				Node* n = *WorldToNode(node.x + x, node.y + y);
				if (n->type == terrain)
				{
					bool found = false;
					for (vector<Node*>::iterator itr = map.begin(); itr != map.end(); itr++)
						if ((*itr) == n)
						{
							found = true;
							break;
						}
					if (!found)
					{
						map.push_back(n);
						n->island = island;
						Expand(n->tile, map, terrain, island);
					}
				}
			}	
}

vector<Node*>* j1PathFinding::GetIsland(fPoint position)
{
	iPoint pos = App->map->WorldToMap(position.x, position.y);

	Node* node = *(WorldToNode(pos.x, pos.y));

	return &node->island->map;
}

vector<fPoint> j1PathFinding::PathTo(fPoint start_pos, fPoint end_pos, NodeType terrain, vector<Node*>* map)
{
	vector<fPoint> ret;

	iPoint _start = App->map->WorldToMap(start_pos.x, start_pos.y);
	iPoint _end = App->map->WorldToMap(end_pos.x, end_pos.y);

	NodeType pathterrain = terrain;

	bool reachable = true;
	if (terrain != NodeType::ALL)
	{
		if (map != nullptr)
		{
			reachable = false;
			for (vector<Node*>::iterator itr = (*map).begin(); itr != (*map).end(); itr++)
				if ((*itr)->tile == _end)
				{
					reachable = true;
					break;
				}
		}
		if ((*WorldToNode(_end.x, _end.y))->type != terrain)
			reachable = false;

		if (!reachable)
		{
			pathterrain = NodeType::ALL;
		}
	}

	if(_end.x >= 0 && _end.y >= 0)
		if (_start == _end)
		{
			ret.push_back(end_pos);
			return ret;
		}
		else
		{
			Node* start = *WorldToNode(_start.x, _start.y);
			Node* end = *WorldToNode(_end.x, _end.y);

			if (start == nullptr || end == nullptr)
				return ret;

			vector<Node*> open;
			vector<Node*> closed;

			open.push_back(start);

			Node* current = nullptr;
			vector<Node*>::iterator current_itr;

			while (open.size() != 0)
			{
				current_itr = open.begin();
				current = *current_itr;
				for (vector<Node*>::iterator i = open.begin(); i != open.end(); i++)
					if (current == nullptr || (*i)->GetFCost() < current->GetFCost() || (*i)->GetFCost() == current->GetFCost() && (*i)->h < current->h)
					{
						current = *i;
						current_itr = i;
					}

				open.erase(current_itr);
				open.shrink_to_fit();

				closed.push_back(current);
				current_itr = closed.end();

				if (current == end)
				{
					ret = CreatePath(current, end_pos, terrain);

					for (vector<Node*>::iterator n = open.begin(); n != open.end(); n++)
						(*n)->Reset();
					for (vector<Node*>::iterator n = closed.begin(); n != closed.end(); n++)
						(*n)->Reset();
					open.erase(open.begin(), open.end());
					open.shrink_to_fit();
					closed.erase(closed.begin(), closed.end());
					closed.shrink_to_fit();

					return ret;
				}

				vector<Node*> neighbours = GetNeighbours(current->tile);
				for (vector<Node*>::iterator i = neighbours.begin(); i != neighbours.end(); i++)
					if (((*i)->type == pathterrain && !(*i)->built) || pathterrain == ALL)
					{
						int g = current->g + DistanceTo(*i, current);
						int h = DistanceTo(*i, end);

						bool push = true;
						for (vector<Node*>::iterator n = closed.begin(); n != closed.end(); n++)
							if (*i == *n)
								if ((g + h) < (*n)->GetFCost())
									continue;
								else
									push = false;

						for (vector<Node*>::iterator n = open.begin(); n != open.end(); n++)
							if (*i == *n)
								if ((g + h) < (*n)->GetFCost())
									continue;
								else
									push = false;

						if (push)
						{
							(*i)->g = g;
							(*i)->h = h;
							(*i)->parent = current;
							open.push_back(*i);
						}

					}
				neighbours.erase(neighbours.begin(), neighbours.end());
				neighbours.shrink_to_fit();
			}
		}
	return ret;
}

vector<Node*> j1PathFinding::GetNeighbours(iPoint node)
{
	vector<Node*> ret;

	for (int y = -1; y <= 1; y++) 
		for (int x = -1; x <= 1; x++)
		{
			if (x == 0 && y == 0)
				continue;
			else if ((node.x + x) >= 0 && (node.x + x) < App->map->mapdata->width && (node.y + y) >= 0 && (node.y + y) < App->map->mapdata->height)
			{
				ret.push_back(*WorldToNode(node.x + x, node.y + y));
			}
		}
	return ret;
}

vector<Node*>::iterator j1PathFinding::WorldToNode(int x, int y)
{
	vector<Node*>::iterator ret;
	if (x >= App->map->mapdata->width || y >= App->map->mapdata->height || x < 0 || y < 0)
		return ret;
	return NodeMap.begin() + (App->map->mapdata->width * y + x);
}

int j1PathFinding::DistanceTo(Node* A, Node* B)
{
	int distance = 0;

	int x_diff = abs(A->tile.x - B->tile.x);
	int y_diff = abs(A->tile.y - B->tile.y);

	int straigth;
	if (A->tile.x > B->tile.x && A->tile.y > B->tile.y || A->tile.x < B->tile.x && A->tile.y < B->tile.y)
		straigth = NODE_HEIGTH;
	else
		straigth = NODE_WIDTH;

	if (x_diff < y_diff)
		distance = straigth * x_diff + NODE_DIAGONAL * (y_diff - x_diff);
	else
		distance = straigth * y_diff + NODE_DIAGONAL * (x_diff - y_diff);

	return distance;
}

vector<fPoint> j1PathFinding::CreatePath(Node* end, fPoint end_pos, NodeType terrain)
{
	vector<fPoint> ret;
	vector<Node*> nodes;

	Node* current = end;
	while (current->parent != nullptr)
	{
		nodes.push_back(current);

		current = current->parent;
	}
	reverse(nodes.begin(), nodes.end());

	for (vector<Node*>::iterator itr = nodes.begin(); itr != nodes.end(); itr++)
	{
		if ((*itr)->type == terrain || terrain == NodeType::ALL)
		{
			fPoint pos = App->map->MapToWorld<fPoint>((*itr)->tile.x, (*itr)->tile.y);
			ret.push_back(pos);
		}
		else
			break;
	}

	return ret;
}

bool j1PathFinding::CleanUp()
{
	for (vector<Island*>::iterator itr = islands.begin(); itr != islands.end(); itr++)
	{
		vector<Node*> n;
		(*itr)->map.swap(n);
		delete* itr;
	}
	for (vector<Node*>::iterator itr = NodeMap.begin(); itr != NodeMap.end(); itr++)
		delete* itr;
	NodeMap.erase(NodeMap.begin(), NodeMap.end());
	NodeMap.shrink_to_fit();
	
	return true;
}