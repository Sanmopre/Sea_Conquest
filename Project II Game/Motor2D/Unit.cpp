#include "j1Entities.h"
#include "p2Log.h"

void j1Unit::GoTo(fPoint destination, NodeType terrain)
{
	path = App->pathfinding->PathTo(this->position, destination, terrain);

	if(path.size() != 0)
		this->destination = *path.begin();
}