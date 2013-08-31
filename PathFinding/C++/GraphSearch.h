
#pragma once

#include "Graph.h"
#include <stack>

class GraphSearch
{
protected:
	Graph* graph;
	int startNode;
	int targetNode;

	int* costSoFar;
	int* previousNode;
	std::vector<int> nodesToCheck;

	int Infinity;
	int InvalidNodeIndex;

    int visitedNodes;

	void Initialize();
	void Release();
	int GetNextNode();

public:
	GraphSearch();
	~GraphSearch();

	virtual bool Search(Graph* graph, int startNode, int targetNode) = 0;
	std::stack<int> GetPath();
    int GetNumberOfVisitedNodes();
};
