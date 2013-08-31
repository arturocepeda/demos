
#pragma once

#include <vector>

struct GraphNode
{
	float PosX;
	float PosY;
};

struct GraphConnection
{
	int DestinyNode;
	int Weight;

	GraphConnection(int destinyNode, int weight) 
		: DestinyNode(destinyNode), Weight(weight) {}
};

class Graph
{
public:
	GraphNode* Nodes;
	std::vector<GraphConnection>* AdjacencyList;
	int NumberOfNodes;

	Graph(int numberOfNodes = 128);
	~Graph();

	virtual void Connect(int nodeA, int nodeB, int weight, bool bidirectional = true);
	bool AlreadyConnected(int nodeA, int nodeB);
};
