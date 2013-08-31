using System;
using System.Collections.Generic;

namespace PathFinding
{
	public class MeshGraph : Graph
	{
		protected int nodesColumns;
		protected int nodesRows;
		protected bool[] unreachableNodes;

		public MeshGraph(int nodesColumns, int nodesRows) 
			: base(nodesColumns * nodesRows)
		{
			this.nodesColumns = nodesColumns;
			this.nodesRows = nodesRows;
			NumberOfNodes = nodesColumns * nodesRows;
			unreachableNodes = new bool[NumberOfNodes];
		}

		public void SetUnreachableNode(int nodeIndex)
		{
			unreachableNodes[nodeIndex] = true;
		}

		public new void Connect(int nodeA, int nodeB, int weight, bool bidirectional = true)
		{
			if(!unreachableNodes[nodeB])
				base.Connect(nodeA, nodeB, weight, bidirectional);
		}

		public void SetNodePositions(float firstX, float firstY, float incrementX, float incrementY)
		{
			int nodeIndex = 0;
			float nodePositionX;
			float nodePositionY = firstY;

			for(int row = 0; row < nodesRows; row++)
			{
				nodePositionX = firstX;

				for(int column = 0; column < nodesColumns; column++)
				{
					Nodes[nodeIndex].posX = nodePositionX;
					Nodes[nodeIndex].posY = nodePositionY;
					nodePositionX += incrementX;
					nodeIndex++;
				}

				nodePositionY += incrementY;
			}
		}

		public void MakeConnections()
		{
			int nodeIndex = 0;

			for(int row = 0; row < nodesRows; row++)
			{
				for(int column = 0; column < nodesColumns; column++)
				{
					if(!unreachableNodes[nodeIndex])
					{
						if(row == 0)
						{
							if(column == 0)
							{
								Connect(nodeIndex, nodeIndex + 1, 10);                // right
								Connect(nodeIndex, nodeIndex + nodesColumns, 10);     // down
								Connect(nodeIndex, nodeIndex + nodesColumns + 1, 14); // down right
							}
							else if(column == (nodesColumns - 1))
							{
								Connect(nodeIndex, nodeIndex - 1, 10);                // left
								Connect(nodeIndex, nodeIndex + nodesColumns - 1, 14); // down left
								Connect(nodeIndex, nodeIndex + nodesColumns, 10);     // down							
							}
							else
							{
								Connect(nodeIndex, nodeIndex - 1, 10);                // left
								Connect(nodeIndex, nodeIndex + 1, 10);                // right
								Connect(nodeIndex, nodeIndex + nodesColumns - 1, 14); // down left
								Connect(nodeIndex, nodeIndex + nodesColumns, 10);     // down
								Connect(nodeIndex, nodeIndex + nodesColumns + 1, 14); // down right
							}
						}
						else if(row == (nodesRows - 1))
						{
							if(column == 0)
							{
								Connect(nodeIndex, nodeIndex - nodesColumns, 10);     // up
								Connect(nodeIndex, nodeIndex - nodesColumns + 1, 14); // up right
								Connect(nodeIndex, nodeIndex + 1, 10);                // right
							}
							else if(column == (nodesColumns - 1))
							{
								Connect(nodeIndex, nodeIndex - nodesColumns - 1, 14); // up left
								Connect(nodeIndex, nodeIndex - nodesColumns, 10);     // up
								Connect(nodeIndex, nodeIndex - 1, 10);                // left
							}
							else
							{
								Connect(nodeIndex, nodeIndex - nodesColumns - 1, 14); // up left
								Connect(nodeIndex, nodeIndex - nodesColumns, 10);     // up
								Connect(nodeIndex, nodeIndex - nodesColumns + 1, 14); // up right
								Connect(nodeIndex, nodeIndex - 1, 10);                // left
								Connect(nodeIndex, nodeIndex + 1, 10);                // right
							}
						}
						else
						{
							if(column == 0)
							{
								Connect(nodeIndex, nodeIndex - nodesColumns, 10);     // up
								Connect(nodeIndex, nodeIndex - nodesColumns + 1, 14); // up right
								Connect(nodeIndex, nodeIndex + 1, 10);                // right
								Connect(nodeIndex, nodeIndex + nodesColumns, 10);     // down
								Connect(nodeIndex, nodeIndex + nodesColumns + 1, 14); // down right
							}
							else if(column == (nodesColumns - 1))
							{
								Connect(nodeIndex, nodeIndex - nodesColumns - 1, 14); // up left
								Connect(nodeIndex, nodeIndex - nodesColumns, 10);     // up
								Connect(nodeIndex, nodeIndex - 1, 10);                // left
								Connect(nodeIndex, nodeIndex + nodesColumns - 1, 14); // down left
								Connect(nodeIndex, nodeIndex + nodesColumns, 10);     // down
							}
							else
							{
								Connect(nodeIndex, nodeIndex - nodesColumns - 1, 14);	// up left
								Connect(nodeIndex, nodeIndex - nodesColumns, 10);     // up
								Connect(nodeIndex, nodeIndex - nodesColumns + 1, 14); // up right
								Connect(nodeIndex, nodeIndex - 1, 10);                // left
								Connect(nodeIndex, nodeIndex + 1, 10);                // right
								Connect(nodeIndex, nodeIndex + nodesColumns - 1, 14); // down left
								Connect(nodeIndex, nodeIndex + nodesColumns, 10);     // down
								Connect(nodeIndex, nodeIndex + nodesColumns + 1, 14); // down right
							}
						}
					}

					nodeIndex++;
				}
			}
		}
	}
}
