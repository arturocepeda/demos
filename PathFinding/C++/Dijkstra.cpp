
#include "Dijkstra.h"

bool Dijkstra::Search(Graph* graph, int startNode, int targetNode)
{
	this->graph = graph;
	this->startNode = startNode;
	this->targetNode = targetNode;

	int currentNode;

	Initialize();

	while(nodesToCheck.size() > 0)
	{
		currentNode = GetNextNode();
        visitedNodes++;

		if(currentNode == InvalidNodeIndex)
			return false;

		if(currentNode == targetNode)
			return true;

		for(unsigned int connection = 0; connection < graph->AdjacencyList[currentNode].size(); connection++)
		{
			GraphConnection currentNodeConnection = graph->AdjacencyList[currentNode][connection];
			int connectedNode = currentNodeConnection.DestinyNode;
			int costToThisNode = costSoFar[currentNode] + currentNodeConnection.Weight;

			if(costSoFar[connectedNode] > costToThisNode)
			{
				costSoFar[connectedNode] = costToThisNode;
				previousNode[connectedNode] = currentNode;
			}
		}
	}

	return false;
}
