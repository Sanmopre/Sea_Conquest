#ifndef _j1PathFinding_
#define _j1PathFinding_

#define NODE_WIDTH 100
#define NODE_HEIGTH 50
#define NODE_DIAGONAL 56

#include "p2Point.h"
#include "j1Module.h"
#include <vector>

using namespace std;

enum NodeType
{
	WATER,
	GROUND,
	ALL
};

struct Node
{
	Node(int x, int y, NodeType type)
	{
		tile.x = x;
		tile.y = y;
		this->type = type;
	}
	iPoint tile;
	NodeType type;

	int g = 0;
	int h = 0;
	Node* parent = nullptr;

	int GetFCost() { return g + h; }

	void Reset()
	{
		g = 0;
		h = 0;
		parent = nullptr;
	}
};

class j1PathFinding : public j1Module
{
public:

	bool Start();

	int map_size;

	vector<Node> NodeMap;

	vector<fPoint> PathTo(fPoint start_pos, fPoint end_pos, NodeType terrain);

	vector<Node*> GetNeighbours(iPoint node);
	vector<Node>::iterator PointToNode(int x, int y, vector<Node>* _grid);
	int DistanceTo(Node* A, Node* B);
	vector<fPoint> CreatePath(Node* end, fPoint end_pos);
};

#endif //_j1PathFinding_

