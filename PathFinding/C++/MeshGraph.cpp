
#include "MeshGraph.h"

MeshGraph::MeshGraph(int nodesColumns, int nodesRows) 
    : Graph(nodesColumns * nodesRows)
{
	this->nodesColumns = nodesColumns;
	this->nodesRows = nodesRows;
	unreachableNodes = new bool[NumberOfNodes];
	memset(unreachableNodes, 0, NumberOfNodes);
}

MeshGraph::~MeshGraph()
{
	delete[] unreachableNodes;
}

void MeshGraph::SetUnreachableNode(int nodeIndex)
{
	unreachableNodes[nodeIndex] = true;
}

void MeshGraph::Connect(int nodeA, int nodeB, int weight, bool bidirectional)
{
	if(!unreachableNodes[nodeB])
		Graph::Connect(nodeA, nodeB, weight, bidirectional);
}

void MeshGraph::SetNodePositions(float firstX, float firstY, float incrementX, float incrementY)
{
	int nodeIndex = 0;
	float nodePositionX;
	float nodePositionY = firstY;

	for(int row = 0; row < nodesRows; row++)
	{
		nodePositionX = firstX;

		for(int column = 0; column < nodesColumns; column++)
		{
			Nodes[nodeIndex].PosX = nodePositionX;
			Nodes[nodeIndex].PosY = nodePositionY;
			nodePositionX += incrementX;
			nodeIndex++;
		}

		nodePositionY += incrementY;
	}
}

void MeshGraph::MakeConnections()
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
						Connect(nodeIndex, nodeIndex - nodesColumns - 1, 14); // up left
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
