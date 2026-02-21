#pragma once
#include "Rigidbody.h"
#include "Graph.h"
#include <queue>

class Boid
{
 public:
	Boid();
	Boid(std::vector<Boid*> flockmates);

	Rigidbody rb;
	ofVec2f current_direction;
	int current_destination = 0;

	ofVec2f destinations[5];

	ofVec2f target;

	float max_acceleration = 200.0f;
	float max_angular_acceleration = 3.0f;

	float target_radius = 10.0f;
	float slow_radius = 500.0f;

	void Update(float dt, SteeringOutput steering, std::vector<Boid*> boids);
	void Draw();

	SteeringOutput DynamicArrive();

	bool HasReachedDestination();

	bool movement_is_kinematic = false;

	// Pathfinding
	int current_path_index = 0;
	std::vector<Vertex> current_path;
	bool is_currently_pathing = false;
};