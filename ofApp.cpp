#include "ofApp.h"
#include "Graph.h"
#include "Pathfinding.h"
#include "Boid.h"

Graph graph_small;
Graph graph_large;

std::vector<Boid*> boids;
std::vector<ofVec2f> breadcrumbs;

float time_between_breadcrumbs = 0.15f;
float breadcrumb_timer;

int num_rows = 9;
int num_cols = 16;
int cell_size = 120;

const int kDijkstras = 0;
const int kAStar = 1;

const int kStraightLine = 0;
const int kManhattan = 1;

// Use this to change the pathfinding algorithm
int pathfinding_algorithm = 1;
// Use this to change the heuristic type
int heuristic = 0;

//--------------------------------------------------------------
void ofApp::setup()
{
	breadcrumb_timer = time_between_breadcrumbs;

	// Divide playable area into graph nodes
	int total_vertices = 0;

	for (int row = 0; row < num_rows; row++)
	{
		for (int col = 0; col < num_cols; col++)
		{
			int x = ((col + 1) * cell_size) - (cell_size / 2);
			int y = ((row + 1) * cell_size) - (cell_size / 2);

			int random = std::rand() % (100 - 1);
			graph_large.AddVertex(Vertex(x, y, row, col, total_vertices, random < 10));

			total_vertices++;
		}
	}

	for (int i = 0; i < graph_large.vertices.size(); i++)
	{
		int x = graph_large.vertices[i].col;
		int y = graph_large.vertices[i].row;
		int id = graph_large.vertices[i].id;
		

		if (x > 0) 
		{
			Vertex v = GetVertex(id - 1);
			if (!v.blocked)
				graph_large.edges.push_back(Edge(id, id - 1, cell_size));
		}

		if (x < num_cols - 1)
		{
			Vertex v = GetVertex(id + 1);
			if (!v.blocked)
				graph_large.edges.push_back(Edge(id, id + 1, cell_size));
		}

		if (y > 0)
		{
			Vertex v = GetVertex(id - num_cols);
			if (!v.blocked)
				graph_large.edges.push_back(Edge(id, id - num_cols, cell_size));
		}

		if (y < num_rows - 1)
		{
			Vertex v = GetVertex(id + num_cols);
			if (!v.blocked)
				graph_large.edges.push_back(Edge(id, id + num_cols, cell_size));
		}
	}

	ofLog(OF_LOG_NOTICE, "------------EDGES-----------");

	for (const auto& edge : graph_large.edges)
	{
		ofLog(OF_LOG_NOTICE, "SRC: " + std::to_string(edge.source) + ", DEST: " + std::to_string(edge.destination));
	}

	Boid* boid = new Boid();
	
	boid->movement_is_kinematic = false;
	boid->rb.position = { 200.0f, 200.0f };

	boids.push_back(boid);

	// Graph 1: Europe 2023 road trip
	// Vertices
	//graph_small.AddVertex({ "Geneva" });
	//graph_small.AddVertex({ "Bern" });
	//graph_small.AddVertex({ "Zurich" });
	//graph_small.AddVertex({ "Vaduz" });
	//graph_small.AddVertex({ "Innsbruck" });
	//graph_small.AddVertex({ "Salzburg" });
	//graph_small.AddVertex({ "Graz" });
	//graph_small.AddVertex({ "Budapest" });
	//graph_small.AddVertex({ "Bratislava" });
	//graph_small.AddVertex({ "Vienna" });
	//graph_small.AddVertex({ "Munich" });

	//// Edges
	//graph_small.AddEdge({ "Geneva", "Bern", 159 });
	//graph_small.AddEdge({ "Bern", "Zurich", 123 });
	//graph_small.AddEdge({ "Zurich", "Vaduz", 109 });
	//graph_small.AddEdge({ "Vaduz", "Innsbruck", 171 });
	//graph_small.AddEdge({ "Innsbruck", "Salzburg", 186 });
	//graph_small.AddEdge({ "Salzburg", "Graz", 279 });
	//graph_small.AddEdge({ "Graz", "Budapest", 346 });
	//graph_small.AddEdge({ "Budapest", "Bratislava", 201 });
	//graph_small.AddEdge({ "Bratislava", "Vienna", 80 });
	//graph_small.AddEdge({ "Vienna", "Munich", 403 });
	//graph_small.AddEdge({ "Munich", "Bern", 585 });
}

//--------------------------------------------------------------
void ofApp::update()
{
	float dt = ofGetLastFrameTime();

	breadcrumb_timer -= dt;

	if (breadcrumb_timer <= 0) 
	{
		breadcrumbs.push_back(boids[0]->rb.position);
		breadcrumbs.push_back(boids[0]->rb.position);
		breadcrumb_timer = time_between_breadcrumbs;
	}

	for (const auto& boid : boids)
	{
		SteeringOutput steering;
		boid->Update(dt, steering, boids);
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(0);

	// Boids
	ofSetColor(255, 255, 255);
	for (const auto& boid : boids)
	{
		boid->Draw();
	}

	// Vertices & walls
	for (const auto& vertex : graph_large.vertices)
	{
		if (!vertex.blocked)
		{
			ofSetColor(255, 255, 255);
			ofDrawBitmapString(std::to_string(vertex.id), vertex.x, vertex.y);
			ofSetColor(255, 255, 255, 25);
			ofDrawCircle({ vertex.x, vertex.y }, 5.0f);
		}
		else
		{
			ofSetColor(50, 50, 50);
			ofSetRectMode(OF_RECTMODE_CENTER);
			ofDrawRectangle(vertex.x, vertex.y, cell_size, cell_size);
		}
	}

	// Breadcrumbs
	ofSetColor(255, 255, 0);
	for (const auto& breadcrumb : breadcrumbs)
	{
		ofDrawCircle(breadcrumb, 2);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
	int current_cell_x = 0;
	int current_cell_y = 0;

	Boid* boid = boids[0];

	current_cell_x = (int)std::floor(boid->rb.position.x / cell_size);
	current_cell_y = (int)std::floor(boid->rb.position.y / cell_size);

	int source_id = 0;

	for (const auto& vertex : graph_large.vertices)
	{
		if (vertex.col == current_cell_x && vertex.row == current_cell_y)
		{
			source_id = vertex.id;
			break;
		}
	}

	int target_cell_x = 0;
	int target_cell_y = 0;

	target_cell_x = (int)std::floor(x / cell_size);
	target_cell_y = (int)std::floor(y / cell_size);

	int target_id = 0;

	// Find the id of this cell in the graph
	for (const auto& vertex : graph_large.vertices)
	{
		if (vertex.col == target_cell_x && vertex.row == target_cell_y)
		{
			target_id = vertex.id;
			break;
		}
	}

	if (pathfinding_algorithm == kAStar)
		boid->current_path = Pathfinding::AStar(heuristic, source_id, target_id, graph_large);
	else
		boid->current_path = Pathfinding::Dijkstras(source_id, target_id, graph_large);

	boid->current_path_index = 0;
	Vertex first_target = boid->current_path[0];
	boid->target = {(float)first_target.x, (float)first_target.y};
	boid->is_currently_pathing = true;

	for (const auto& vertex : boid->current_path)
	{
		ofLog(OF_LOG_NOTICE, "Vertex: " + std::to_string(vertex.id));
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

Vertex ofApp::GetVertex(int id)
{
	for (const auto& vertex : graph_large.vertices)
	{
		if (vertex.id == id)
			return vertex;
	}
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{ 

}
