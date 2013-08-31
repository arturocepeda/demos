
#pragma once

#include "Graph.h"

class MeshGraph : public Graph
{
protected:
	int nodesColumns;
	int nodesRows;
	bool* unreachableNodes;

public:
	MeshGraph(int nodesColumns, int nodesRows);
	~MeshGraph();

	void SetUnreachableNode(int nodeIndex);
	void Connect(int nodeA, int nodeB, int weight, bool bidirectional = true);
	void SetNodePositions(float firstX, float firstY, float incrementX, float incrementY);
	void MakeConnections();
};
