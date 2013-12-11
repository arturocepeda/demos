
////////////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Pathfinding
//
//  --- MeshGraph.h ---
//
////////////////////////////////////////////////////////////////////////

#pragma once

#include "Graph.h"

class MeshGraph : public Graph
{
protected:
    static const int LineWeight;
    static const int DiagonalWeight;

	int nodesColumns;
	int nodesRows;
    
    void setConnections(int nodeIndex, int column, int row);

public:
	MeshGraph(int nodesColumns, int nodesRows);
	~MeshGraph();

    int getNumberOfColumns();
    int getNumberOfRows();

    void setConnections();
    void resetConnections();

	void setNodePositions(float firstX, float firstY, float incrementX, float incrementY);
};
