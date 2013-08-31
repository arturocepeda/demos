using System;
using System.Collections.Generic;

namespace PathFinding
{
	class Program
	{
		static void Main(string[] args)
		{
			MeshGraph meshGraph = new MeshGraph(13, 10);
			new ParserMeshGraph().LoadUnreachableNodesFromFile(meshGraph, "..\\..\\..\\testobstacles.txt");
			meshGraph.MakeConnections();
			meshGraph.SetNodePositions(0.0f, 0.0f, 0.01f, 0.01f);

			Dijkstra dijkstra = new Dijkstra();
			AStar astar = new AStar();

			int begin = 129;
			int end = 0;

			Console.Write("\n\n  (Dijkstra)");
			TryAlgorithm(meshGraph, begin, end, dijkstra);
			Console.Write("\n\n  (A*)");
			TryAlgorithm(meshGraph, begin, end, astar);
			Console.ReadKey(true);
		}

		static void TryAlgorithm(Graph graph, int nodeStart, int nodeTarget, GraphSearch algorithm)
		{
			Console.Write("\n   Path from " + nodeStart + " to " + nodeTarget + ":");

			bool pathFound = algorithm.Search(graph, nodeStart, nodeTarget);

			if(pathFound)
			{
				Stack<int> path = algorithm.GetPath();

				while (path.Count > 0)
					Console.Write(" " + path.Pop());

                Console.WriteLine("\n   " + algorithm.GetNumberOfVisitedNodes() + " nodes visited\n");
			}
			else
			{
				Console.Write(" no path");
			}
		}
	}
}
