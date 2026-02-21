#pragma once

#include <string>
#include <vector>
#include <map>

class Edge
{
 public:
	Edge() = default;
	Edge::Edge(int src, int dest, int edge_weight) : source(src), destination(dest), weight(edge_weight) {}

	int source;
	int destination;
	int weight;
};

class Vertex
{
 public:
	Vertex() = default;
	Vertex(int vert_x, int vert_y, int vert_row, int vert_col, int vert_id, bool is_blocked) 
		: row(vert_row), 
		col(vert_col), 
		x(vert_x), 
		y(vert_y), 
		id(vert_id),
		blocked(is_blocked) {}

	int row, col;
	int x, y;
	int id;

	bool blocked;
};

class Graph
{
 public:
	Graph() = default;

	std::vector<Edge> edges;
	std::vector<Vertex> vertices;

	void AddEdge(Edge new_edge);
	void AddVertex(Vertex new_vert);
};