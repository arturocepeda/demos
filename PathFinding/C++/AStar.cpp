
////////////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Pathfinding
//
//  --- AStar.cpp ---
//
////////////////////////////////////////////////////////////////////////

#include "AStar.h"

const float AStar::HeuristicWeight = 25.0f;

int AStar::estimateDistance(int nodeFrom, int nodeTo)
{
    return (int)((fabs(graph->Nodes[nodeTo].PosX - graph->Nodes[nodeFrom].PosX) +
        fabs(graph->Nodes[nodeTo].PosY - graph->Nodes[nodeFrom].PosY)) * HeuristicWeight);
}
