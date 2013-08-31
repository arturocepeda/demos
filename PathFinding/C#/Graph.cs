using System;
using System.Collections.Generic;

namespace PathFinding
{
	public struct GraphNode
	{
		public float posX;
		public float posY;
	}

	public struct GraphConnection
	{
		public int destinyNode;
		public int weight;

		public GraphConnection(int destinyNode, int weight)
		{
			this.destinyNode = destinyNode;
			this.weight = weight;
		}
	}

	public class Graph
	{
		public GraphNode[] Nodes { get; set; }
		public List<GraphConnection>[] AdjacencyList { get; protected set; }
		public int NumberOfNodes { get; protected set; }

		public Graph(int numberOfNodes)
		{
			this.NumberOfNodes = numberOfNodes;
			Nodes = new GraphNode[numberOfNodes];
			AdjacencyList = new List<GraphConnection>[numberOfNodes];

			for(int i = 0; i < numberOfNodes; i++)
				AdjacencyList[i] = new List<GraphConnection>();
		}

		public void Connect(int nodeA, int nodeB, int weight, bool bidirectional = true)
		{
			if(!AlreadyConnected(nodeA, nodeB))
				AdjacencyList[nodeA].Add(new GraphConnection(nodeB, weight));

			if(bidirectional && !AlreadyConnected(nodeB, nodeA))
				AdjacencyList[nodeB].Add(new GraphConnection(nodeA, weight));
		}

		public bool AlreadyConnected(int nodeA, int nodeB)
		{
			for(int i = 0; i < AdjacencyList[nodeA].Count; i++)
			{
				if(AdjacencyList[nodeA][i].destinyNode == nodeB)
					return true;
			}

			return false;
		}
	}
}
