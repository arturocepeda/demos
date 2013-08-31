
#include "GraphSearch.h"
#include <vector>
#include <climits>

void GraphSearch::Initialize()
{
	Release();
	costSoFar = new int[graph->NumberOfNodes];
	previousNode = new int[graph->NumberOfNodes];
	nodesToCheck.clear();
    visitedNodes = 0;

	for(int i = 0; i < graph->NumberOfNodes; i++)
	{
		costSoFar[i] = Infinity;
		previousNode[i] = InvalidNodeIndex;
		nodesToCheck.push_back(i);
	}

	costSoFar[startNode] = 0;
}

void GraphSearch::Release()
{
	if(costSoFar != NULL)
	{
		delete[] costSoFar;
		costSoFar = NULL;
	}

	if(previousNode != NULL)
	{
		delete[] previousNode;
		previousNode = NULL;
	}
}

int GraphSearch::GetNextNode()
{
	int minCost = Infinity;
	int nextNode = InvalidNodeIndex;
	int nextNodeIndexInTheList;
	int currentNode;
	int cost;

	for(unsigned int i = 0; i < nodesToCheck.size(); i++)
	{
		currentNode = nodesToCheck[i];
		cost = costSoFar[currentNode];

		if(cost < minCost)
		{
			minCost = costSoFar[currentNode];
			nextNode = currentNode;
			nextNodeIndexInTheList = i;
		}
	}

	nodesToCheck.erase(nodesToCheck.begin() + nextNodeIndexInTheList);

	return nextNode;
}

GraphSearch::GraphSearch()
{
	Infinity = std::numeric_limits<int>::max();
	InvalidNodeIndex = -1;
	costSoFar = NULL;
	previousNode = NULL;
}

GraphSearch::~GraphSearch()
{
	Release();
}

std::stack<int> GraphSearch::GetPath()
{
	std::stack<int> shortestPath;
	int currentNode = targetNode;

	while(currentNode != startNode)
	{
		shortestPath.push(currentNode);
		currentNode = previousNode[currentNode];
	}

	shortestPath.push(currentNode);

	return shortestPath;
}

int GraphSearch::GetNumberOfVisitedNodes()
{
    return visitedNodes;
}
