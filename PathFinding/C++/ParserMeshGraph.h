
////////////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Pathfinding
//
//  --- ParserMeshGraph.h ---
//
////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include "MeshGraph.h"

class ParserMeshGraph
{
private:
    std::string loadTextContents(std::string filename);
    std::vector<std::string> splitString(const std::string& str, char delimiter);

public:
    void loadUnreachableNodesFromFile(MeshGraph* meshGraph, std::string filename);
};
