#include "Graph.h"

void Graph::AddEdge(Edge new_edge)
{
	edges.push_back(new_edge);
}

void Graph::AddVertex(Vertex new_vert)
{
	vertices.push_back(new_vert);
}
