#include "j1Entities.h"
#include "p2Log.h"

j1Unit::~j1Unit()
{
	path.erase(path.begin(), path.end());
	path.shrink_to_fit();
}

void j1Unit::GoTo(fPoint destination, NodeType terrain)
{
	path.swap(App->pathfinding->PathTo(this->position, destination, terrain));

	if(path.size() != 0)
		this->destination = *path.begin();
}