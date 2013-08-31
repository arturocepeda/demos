
#pragma once

#include <string>
#include "MeshGraph.h"

class ParserMeshGraph
{
private:
	std::string LoadTextContents(std::string filename);
	std::vector<std::string> SplitString(const std::string& str, char delimiter);

public:
	void LoadUnreachableNodesFromFile(MeshGraph* meshGraph, std::string filename);
};
