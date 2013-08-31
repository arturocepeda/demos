
#include <iostream>
#include "MeshGraph.h"
#include "ParserMeshGraph.h"
#include "Dijkstra.h"
#include "AStar.h"
#include <stack>

void TryAlgorithm(Graph& graph, int nodeStart, int nodeTarget, GraphSearch& algorithm);

int main(int argc, char* argv[])
{
	MeshGraph meshGraph(13, 10);
	ParserMeshGraph parserMeshGraph;

	parserMeshGraph.LoadUnreachableNodesFromFile(&meshGraph, ".\\..\\testobstacles.txt");
	meshGraph.MakeConnections();
	meshGraph.SetNodePositions(0.0f, 0.0f, 0.01f, 0.01f);

	Dijkstra dijkstra;
	AStar astar;

	int begin = 129;
	int end = 0;

	std::cout << "\n\n  (Dijkstra)";
	TryAlgorithm(meshGraph, begin, end, dijkstra);
	std::cout << "\n\n  (A*)";
	TryAlgorithm(meshGraph, begin, end, astar);

	std::cin.get();
	return 0;
}

void TryAlgorithm(Graph& graph, int nodeStart, int nodeTarget, GraphSearch& algorithm)
{
	std::cout << "\n   Path from " << nodeStart << " to " << nodeTarget << ":";
	bool pathFound = algorithm.Search(&graph, nodeStart, nodeTarget);

	if(pathFound)
	{
		std::stack<int> path = algorithm.GetPath();

		while(path.size() > 0)
		{
			std::cout << " " << path.top();
			path.pop();
		}

        std::cout << "\n   " << algorithm.GetNumberOfVisitedNodes() << " nodes visited\n";
	}
	else
	{
		std::cout << " no path";
	}
}
