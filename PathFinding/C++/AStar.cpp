
#include "AStar.h"

const float AStar::HeuristicWeight = 2500.0f;

int AStar::EstimateDistance(int nodeFrom, int nodeTo)
{
	return (int)((fabs(graph->Nodes[nodeTo].PosX - graph->Nodes[nodeFrom].PosX) +
		fabs(graph->Nodes[nodeTo].PosY - graph->Nodes[nodeFrom].PosY)) * HeuristicWeight);
}

bool AStar::Search(Graph* graph, int startNode, int targetNode)
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
			int costToThisNode = costSoFar[currentNode] + currentNodeConnection.Weight +
				EstimateDistance(connectedNode, targetNode);

			if(costSoFar[connectedNode] > costToThisNode)
			{
				costSoFar[connectedNode] = costToThisNode;
				previousNode[connectedNode] = currentNode;
			}
		}
	}

	return false;
}
