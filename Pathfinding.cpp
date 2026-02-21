#include "Pathfinding.h"
#include <queue>

#define INF std::numeric_limits<int>::max()
typedef std::pair<int, int> PQPair; // Used for priority queue: <weight, vertex>

std::vector<Vertex> Pathfinding::Dijkstras(int source, int destination, Graph graph)
{
    std::priority_queue<PQPair, std::vector<PQPair>, std::greater<>> pq;
    std::map<int, int> distance;
    std::map<int, int> visited;

    for (const auto& vertex : graph.vertices)
    {
		distance[vertex.id] = INF;
		visited[vertex.id] = -1;
    }

    distance[source] = 0;
    pq.push({ 0, source });

    while (!pq.empty())
    {
        int current = pq.top().second;
        int current_distance = pq.top().first;
        pq.pop();

        if (current_distance > distance[current])
            continue;

        if (current == destination)
            break;
        
        for (const auto& edge : graph.edges)
        {
            if (edge.source == current)
            {
                int new_distance = distance[current] + edge.weight;
                if (new_distance < distance[edge.destination])
                {
                    distance[edge.destination] = new_distance;
                    visited[edge.destination] = current;
                    pq.push({ distance[edge.destination], edge.destination });
                }
            }
        }
    }

    std::vector<Vertex> path;
    int current = destination;
    
    while (current != -1 && current != source)
    {
		for (const auto& vertex : graph.vertices)
		{
			if (vertex.id == current)
			{
				path.push_back(vertex);
			}
		}

		current = visited[current];
    }

    if (current == -1)
    {
        // oh no!
        return {};
    }
    
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<Vertex> Pathfinding::AStar(int heuristic_type, int source, int destination, Graph graph)
{
    std::priority_queue<PQPair, std::vector<PQPair>, std::greater<>> pq;
    std::map<int, int> g_values;
    std::map<int, int> f_values;
    std::map<int, int> came_from;

    for (const auto& vertex : graph.vertices)
    {
        g_values[vertex.id] = INF;
        f_values[vertex.id] = INF;
    }

    g_values[source] = 0;
    f_values[source] = Heuristic(heuristic_type, source, destination, graph);

    pq.push({ f_values[source], source });

    while (!pq.empty())
    {
        int current = pq.top().second;
        pq.pop();

        if (current == destination)
            break;

        for (const auto& edge : graph.edges)
        {
			if (edge.source == current) 
            {
				int tentative_g_value = g_values[current] + edge.weight;

				if (tentative_g_value < g_values[edge.destination]) 
                {
					came_from[edge.destination] = current;
					g_values[edge.destination] = tentative_g_value;
					f_values[edge.destination] = g_values[edge.destination] + Heuristic(heuristic_type, edge.destination, destination, graph);
					pq.push({ f_values[edge.destination], edge.destination });
				}
			}
        }
    }

    
    std::vector<Vertex> path;
    int current = destination;
    
    while (current != source)
    {
        for (const auto& vertex : graph.vertices)
        {
            if (vertex.id == current)
            {
                path.push_back(vertex);
            }
        }

        current = came_from[current];
    }

    std::reverse(path.begin(), path.end());

    return path;
}

int Pathfinding::Heuristic(const int heuristic_type, const int source, const int destination, Graph graph)
{
    // Straight-line
    if (heuristic_type == 0)
        return Heuristic_StraightLine(source, destination, graph);
    else
        return Heuristic_Manhattan(source, destination, graph);
}

int Pathfinding::Heuristic_StraightLine(const int source, const int destination, Graph graph)
{
    ofVec2f source_location = { -999.0f, -999.0f };
    ofVec2f dest_location = { -999.0f, -999.0f };

    for (const auto& vertex : graph.vertices)
    {
        if (vertex.id == source)
            source_location = { (float)vertex.x, (float)vertex.y };
        else if (vertex.id == destination)
            dest_location = { (float)vertex.x, (float)vertex.y };

        if (source_location.x != -999.0f && dest_location.x != -999.0f)
            break;
    }

    return source_location.distance(dest_location);
}

int Pathfinding::Heuristic_Manhattan(const int source, const int destination, Graph graph)
{
	ofVec2f source_location = { -999.0f, -999.0f };
	ofVec2f dest_location = { -999.0f, -999.0f };

	for (const auto& vertex : graph.vertices)
	{
		if (vertex.id == source)
			source_location = { (float)vertex.x, (float)vertex.y };
		else if (vertex.id == destination)
			dest_location = { (float)vertex.x, (float)vertex.y };

		if (source_location.x != -999.0f && dest_location.x != -999.0f)
			break;
	}

    int x = std::abs(dest_location.x - source_location.x);
    int y = std::abs(dest_location.y - source_location.y);
    return x + y;
}
