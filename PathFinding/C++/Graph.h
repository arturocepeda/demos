
////////////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Pathfinding
//
//  --- Graph.h ---
//
////////////////////////////////////////////////////////////////////////

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
    bool Active;

	GraphConnection(int destinyNode, int weight) 
		: DestinyNode(destinyNode)
        , Weight(weight) 
        , Active(true) {}
};

class Graph
{
protected:
    bool* unreachableNodes;

    void setConnectionActive(int nodeA, int nodeB, bool active);
    GraphConnection* getConnection(int nodeA, int nodeB);

public:
	GraphNode* Nodes;
	std::vector<GraphConnection>* AdjacencyList;
	unsigned int NumberOfNodes;

	Graph(int numberOfNodes = 128);
	~Graph();
    
    void setReachableNode(int nodeIndex);
	void setUnreachableNode(int nodeIndex);
    bool isUnreachableNode(int nodeIndex);

	void connect(int nodeA, int nodeB, int weight, bool bidirectional = true);
	bool alreadyConnected(int nodeA, int nodeB);

    void setConnectionWeight(int nodeA, int nodeB, int weight);
    void activateConnection(int nodeA, int nodeB);
    void deactivateConnection(int nodeA, int nodeB);
};
