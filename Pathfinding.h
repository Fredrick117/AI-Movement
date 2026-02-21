#pragma once
#include <vector>
#include "Graph.h"
#include "ofMain.h"

class Pathfinding
{
 public:
	static std::vector<Vertex> Dijkstras(int source, int destination, Graph graph);
	static std::vector<Vertex> AStar(int heuristic_type, int source, int destination, Graph graph);

 private:
	static int Heuristic(const int heuristic_type, const int source, const int destination, Graph graph);
	static int Heuristic_StraightLine(const int source, const int destination, Graph graph);
	static int Heuristic_Manhattan(const int source, const int destination, Graph graph);
};