
#pragma once

#include "GraphSearch.h"

class Dijkstra : public GraphSearch
{
public:
	bool Search(Graph* graph, int startNode, int targetNode);
};
