
#pragma once

#include "GraphSearch.h"
#include <cmath>

class AStar : public GraphSearch
{
private:
    static const float HeuristicWeight;

	int EstimateDistance(int nodeFrom, int nodeTo);

public:
	bool Search(Graph* graph, int startNode, int targetNode);
};
