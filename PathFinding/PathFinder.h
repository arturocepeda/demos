
////////////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Pathfinding
//
//  --- PathFinder.h ---
//
////////////////////////////////////////////////////////////////////////

#pragma once

#include "Graph.h"
#include "GraphSearch.h"
#include <vector>

class PathFinder
{
private:
    Graph* graph;
    GraphSearch* algorithm;

    int currentStartNode;
    int currentTargetNode;
    std::vector<int> currentPath;

    int getPositionOfNode(int node);

public:
    PathFinder(Graph* graph, GraphSearch* algorithm);
    ~PathFinder();

    std::vector<int>& getCurrentPath();

    bool calculatePath(int nodeStart, int nodeTarget);
    bool updatePath(int currentNode, int updatedNode);
};
