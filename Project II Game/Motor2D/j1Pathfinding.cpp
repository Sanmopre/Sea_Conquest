#include "j1App.h"
#include "j1Pathfinding.h"

#include "j1Render.h"
#include "j1Map.h"
#include "p2Log.h"

bool j1PathFinding::Start()
{
	return true;
}

vector<fPoint> j1PathFinding::PathTo(fPoint start_pos, fPoint end_pos, NodeType terrain)
{
	vector<fPoint> ret;

	iPoint _start = App->map->WorldToMap((int)start_pos.x, (int)start_pos.y);
	iPoint _end = App->map->WorldToMap((int)end_pos.x, (int)end_pos.y);

	if (_start == _end)
	{
		ret.push_back(end_pos);
		return ret;
	}
	else
	{
		Node* start = &*PointToNode(_start.x, _start.y, &NodeMap);
		Node* end = &*PointToNode(_end.x, _end.y, &NodeMap);

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
				ret = CreatePath(current, end_pos);

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
				if ((*i)->type == terrain || terrain == ALL)
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
			else if ((node.x + x) >= 0 && (node.x + x) < map_size && (node.y + y) >= 0 && (node.y + y) < map_size)
			{
				ret.push_back(&*PointToNode((node.x + x), (node.y + y), &NodeMap));
			}
		}
	return ret;
}

vector<Node>::iterator j1PathFinding::PointToNode(int x, int y, vector<Node>* _grid)
{
	vector<Node>::iterator ret;
	if (x >= map_size || y >= map_size || x < 0 || y < 0)
		return ret;
	return _grid->begin() + (map_size * y + x);
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

vector<fPoint> j1PathFinding::CreatePath(Node* end, fPoint end_pos)
{
	vector<fPoint> ret;
	ret.push_back(end_pos);
	Node* current = end;
	while (current->parent != nullptr)
	{
		current = current->parent;
		ret.push_back(App->map->MapToWorld<fPoint>(current->tile.x, current->tile.y));

	}

	ret.erase(ret.end() - 1);
	ret.shrink_to_fit();

	reverse(ret.begin(), ret.end());

	return ret;
}

bool j1PathFinding::CleanUp()
{
	NodeMap.erase(NodeMap.begin(), NodeMap.end());
	NodeMap.shrink_to_fit();
	
	return true;
}