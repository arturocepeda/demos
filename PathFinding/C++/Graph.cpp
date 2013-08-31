
#include "Graph.h"

Graph::Graph(int numberOfNodes)
{
	NumberOfNodes = numberOfNodes;
	Nodes = new GraphNode[numberOfNodes];
	AdjacencyList = new std::vector<GraphConnection>[numberOfNodes];
}

Graph::~Graph()
{
	delete[] Nodes;
	delete[] AdjacencyList;
}

void Graph::Connect(int nodeA, int nodeB, int weight, bool bidirectional)
{
	if(!AlreadyConnected(nodeA, nodeB))
	{
		GraphConnection connection(nodeB, weight);
		AdjacencyList[nodeA].push_back(connection);
	}

	if(bidirectional && !AlreadyConnected(nodeB, nodeA))
	{
		GraphConnection connection(nodeA, weight);
		AdjacencyList[nodeB].push_back(connection);
	}
}

bool Graph::AlreadyConnected(int nodeA, int nodeB)
{
	for(unsigned int i = 0; i < AdjacencyList[nodeA].size(); i++)
	{
		if(AdjacencyList[nodeA][i].DestinyNode == nodeB)
			return true;
	}

	return false;
}
