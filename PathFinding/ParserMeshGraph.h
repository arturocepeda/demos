
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
#include "PathFinding/GEMeshGraph.h"

class ParserMeshGraph
{
private:
   std::string loadTextContents(std::string filename);
   std::vector<std::string> splitString(const std::string& str, char delimiter);

public:
   void loadUnreachableNodesFromFile(GE::Pathfinding::MeshGraph* meshGraph, std::string filename);
};
