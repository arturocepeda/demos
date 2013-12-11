
////////////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Pathfinding
//
//  (Test application)
//
//  --- main.cpp ---
//
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include "MeshGraph.h"
#include "ParserMeshGraph.h"
#include "Dijkstra.h"
#include "AStar.h"
#include <vector>
#include <windows.h>

const int MeshColumns = 13;
const int MeshRows = 10;

const int ColorBlue = 0x01;
const int ColorGreen = 0x02;
const int ColorRed = 0x04;
const int ColorYellow = ColorGreen | ColorRed;
const int ColorWhite = ColorBlue | ColorGreen | ColorRed;

HANDLE hstdout;

std::vector<int> tryAlgorithm(Graph& graph, int nodeStart, int nodeTarget, GraphSearch* algorithm);
void drawGraph(MeshGraph& graph, const std::vector<int>& path);
bool contains(const std::vector<int>& path, int node);

int main(int argc, char* argv[])
{
    MeshGraph meshGraph(MeshColumns, MeshRows);
    ParserMeshGraph parserMeshGraph;

    parserMeshGraph.loadUnreachableNodesFromFile(&meshGraph, ".\\..\\testobstacles.txt");
    meshGraph.setNodePositions(0.0f, 0.0f, 1.0f, 1.0f);
    meshGraph.setConnections();

    GraphSearch* searchAlgorithm = new AStar();
    std::vector<int> path;

    int start = 0;
    int lastNode = (MeshColumns * MeshRows) - 1;
    int end = lastNode;

    int option;
    int nodeA;
    int nodeB;

    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

    do
    {
        SetConsoleTextAttribute(hstdout, ColorGreen);
        path = tryAlgorithm(meshGraph, start, end, searchAlgorithm);

        drawGraph(meshGraph, path);

        std::cout << "\n\n   0) Exit  1) Select route  2) Open node  3) Close node -> ";
        std::cin >> option;

        if(option == 1)
        {
            std::cout << "\n   Node A: ";
            std::cin >> nodeA;
            std::cout << "   Node B: ";
            std::cin >> nodeB;
        }
        else if(option >= 2 && option <= 3)
        {
            std::cout << "\n   Node: ";
            std::cin >> nodeA;
        }

        switch(option)
        {
        case 1:
            if(nodeA >= 0 && nodeA <= lastNode)
                start = nodeA;
            if(nodeB >= 0 && nodeB <= lastNode)
                end = nodeB;
            break;
        case 2:
            if(nodeA >= 0 && nodeA <= lastNode)
                meshGraph.setReachableNode(nodeA);
            break;
        case 3:
            if(nodeA >= 0 && nodeA <= lastNode)
                meshGraph.setUnreachableNode(nodeA);
            break;
        }

    } while (option != 0);

    delete searchAlgorithm;

    return 0;
}

void drawGraph(MeshGraph& graph, const std::vector<int>& path)
{
    for(int i = 0; i < graph.getNumberOfRows(); i++)
    {
        std::cout << "\n      ";

        for(int j = 0; j < graph.getNumberOfColumns(); j++)
        {
            int node = i * graph.getNumberOfColumns() + j;

            if(graph.isUnreachableNode(node))
                SetConsoleTextAttribute(hstdout, ColorRed);
            else if(contains(path, node))
                SetConsoleTextAttribute(hstdout, ColorGreen);
            else
                SetConsoleTextAttribute(hstdout, ColorWhite);
                
            std::cout << std::setw(4) << node;
        }

        std::cout << "\n";
    }

    SetConsoleTextAttribute(hstdout, ColorWhite);
}

std::vector<int> tryAlgorithm(Graph& graph, int nodeStart, int nodeTarget, GraphSearch* algorithm)
{
    std::cout << "\n   Path from " << nodeStart << " to " << nodeTarget << ":";
    bool pathFound = algorithm->search(&graph, nodeStart, nodeTarget);
    std::vector<int> path;

    if(pathFound)
    {
        path = algorithm->getPath();
        int currentNode = path.size() - 1;

        while(currentNode >= 0)
            std::cout << " " << path[currentNode--];

        std::cout << "\n   " << algorithm->getNumberOfVisitedNodes() << " nodes visited\n";
    }
    else
    {
        std::cout << " no path\n";
    }

    return path;
}

bool contains(const std::vector<int>& path, int node)
{
    for(unsigned int i = 0; i < path.size(); i++)
    {
        if(path[i] == node)
            return true;
    }

    return false;
}
