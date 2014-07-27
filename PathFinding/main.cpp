
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
#include "PathFinding/GEMeshGraph.h"
#include "PathFinding/GEDijkstra.h"
#include "PathFinding/GEAStar.h"
#include "PathFinding/GEPathFinder.h"
#include "ParserMeshGraph.h"
#include <vector>
#include <windows.h>

using namespace GE::Pathfinding;

const int MeshColumns = 13;
const int MeshRows = 10;

const int ColorBlue = 0x01;
const int ColorGreen = 0x02;
const int ColorRed = 0x04;
const int ColorYellow = ColorGreen | ColorRed;
const int ColorWhite = ColorBlue | ColorGreen | ColorRed;

HANDLE hstdout;

void drawGraph(MeshGraph& graph, const std::vector<int>& path, int currentNode);
void printPath(PathFinder& pathFinder, int nodeStart, int nodeTarget);
bool contains(const std::vector<int>& path, int node);

int main(int argc, char* argv[])
{
   MeshGraph meshGraph(MeshColumns, MeshRows);
   ParserMeshGraph parserMeshGraph;

   parserMeshGraph.loadUnreachableNodesFromFile(&meshGraph, "testobstacles.txt");
   meshGraph.setNodePositions(0.0f, 0.0f, 1.0f, 1.0f);
   meshGraph.setConnections();

   GraphSearch* searchAlgorithm = new AStar(25.0f);

   int start = 0;
   int lastNode = (MeshColumns * MeshRows) - 1;
   int end = lastNode;

   int currentIndex = -1;
   int currentNode;

   PathFinder pathFinder(&meshGraph, searchAlgorithm);
   pathFinder.calculatePath(start, end);

   int option;
   int nodeA;
   int nodeB;

   hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

   do
   {
      SetConsoleTextAttribute(hstdout, ColorGreen);
      std::vector<int>& path = pathFinder.getCurrentPath();

      currentNode = currentIndex >= 0 ? path[currentIndex] : start;

      printPath(pathFinder, start, end);
      drawGraph(meshGraph, path, currentNode);

      std::cout << "\n\n   0) Exit  1) New route  2) Open node  3) Close node  4) Move forward -> ";
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
      case 1: // new route

         if(nodeA >= 0 && nodeA <= lastNode)
            start = nodeA;

         if(nodeB >= 0 && nodeB <= lastNode)
            end = nodeB;

         pathFinder.calculatePath(start, end);
         currentIndex = -1;

         break;
            
      case 2: // open node

         if(nodeA >= 0 && nodeA <= lastNode)
         {
            meshGraph.setReachableNode(nodeA);
            pathFinder.updatePath(currentNode, nodeA);
         }

         break;

      case 3: // close node

         if(nodeA >= 0 && nodeA <= lastNode)
         {
            meshGraph.setUnreachableNode(nodeA);
            pathFinder.updatePath(currentNode, nodeA);
         }

         break;

      case 4: // move forward

         if((currentIndex + 1) < (int)path.size())
            currentIndex++;

         break;
      }

   } while(option != 0);

   delete searchAlgorithm;

   return 0;
}

void drawGraph(MeshGraph& graph, const std::vector<int>& path, int currentNode)
{
   for(int i = 0; i < graph.getNumberOfRows(); i++)
   {
      std::cout << "\n      ";

      for(int j = 0; j < graph.getNumberOfColumns(); j++)
      {
         int node = i * graph.getNumberOfColumns() + j;

         if(node == currentNode)
               SetConsoleTextAttribute(hstdout, ColorYellow);
         else if(graph.isUnreachableNode(node))
               SetConsoleTextAttribute(hstdout, ColorRed);
         else if(contains(path, node))
               SetConsoleTextAttribute(hstdout, ColorGreen);
         else
               SetConsoleTextAttribute(hstdout, ColorWhite);
                
         std::cout << std::setw(4) << node;
      }

      std::cout << std::endl;
   }

   SetConsoleTextAttribute(hstdout, ColorWhite);
}

void printPath(PathFinder& pathFinder, int nodeStart, int nodeTarget)
{
   std::cout << "\n   Path from " << nodeStart << " to " << nodeTarget << ":";
   std::vector<int>& path = pathFinder.getCurrentPath();

   if(!path.empty())
   {
      unsigned int currentNode = 0;

      while(currentNode < path.size())
         std::cout << " " << path[currentNode++];
   }
   else
   {
      std::cout << " no path";
   }

   std::cout << std::endl;
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
