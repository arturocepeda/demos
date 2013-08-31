using System;
using System.Collections.Generic;

namespace PathFinding
{
	public class AStar : GraphSearch
	{
        private const float HeuristicWeight = 2500.0f;

		public override bool Search(Graph graph, int startNode, int targetNode)
		{
			this.graph = graph;
			this.startNode = startNode;
			this.targetNode = targetNode;

			int currentNode;

			Initialize();

			while(nodesToCheck.Count > 0)
			{
				currentNode = GetNextNode();
                visitedNodes++;

				if(currentNode == InvalidNodeIndex)
					return false;

				if(currentNode == targetNode)
					return true;

				for(int connection = 0; connection < graph.AdjacencyList[currentNode].Count; connection++)
				{
					GraphConnection currentNodeConnection = graph.AdjacencyList[currentNode][connection];
					int connectedNode = currentNodeConnection.destinyNode;
					int costToThisNode = costSoFar[currentNode] + currentNodeConnection.weight +
						EstimateDistance(connectedNode, targetNode);

					if(costSoFar[connectedNode] > costToThisNode)
					{
						costSoFar[connectedNode] = costToThisNode;
						previousNode[connectedNode] = currentNode;
					}
				}
			}

			return false;
		}

		private int EstimateDistance(int nodeFrom, int nodeTo)
		{
			return (int)((Math.Abs(graph.Nodes[nodeTo].posX - graph.Nodes[nodeFrom].posX) +
				Math.Abs(graph.Nodes[nodeTo].posY - graph.Nodes[nodeFrom].posY)) * HeuristicWeight);
		}
	}
}
