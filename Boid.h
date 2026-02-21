#pragma once

#include "Rigidbody.h"

enum BehaviorType
{
	SEEK,
	ARRIVE,
	ARRIVE2,
	WANDER,
	WANDER2,
	FLOCK
};

class Boid
{
public:
	Boid();
	Boid(std::vector<Boid*> _flockmates);

	Rigidbody rb;
	ofVec2f currentDirection;
	int currentDestination = 0;

	ofVec2f destinations[5];

	ofVec2f target;

	float maxAcceleration = 200.0f;
	float maxAngularAcceleration = 3.0f;

	float targetRadius = 10.0f;
	float slowRadius = 500.0f;

	void Update(float dt, SteeringOutput steering, std::vector<Boid*> boids);
	void Draw();

	SteeringOutput DynamicSeek();
	SteeringOutput DynamicArrive();
	SteeringOutput ArriveV2();
	SteeringOutput Wander();
	SteeringOutput WanderV2();
	SteeringOutput Flock(std::vector<Boid*> boids);

	SteeringOutput Alignment(std::vector<Boid*> boids);
	SteeringOutput Separation(std::vector<Boid*> boids);
	SteeringOutput Cohesion(std::vector<Boid*> boids);

	bool HasReachedDestination();

	/////////////////////////////
	// PER-BEHAVIOR PROPERTIES //
	/////////////////////////////

	bool movementIsKinematic = false;

	BehaviorType behaviorType = SEEK;

	// Wander
	float wanderAngle = PI / 2;
	ofVec2f wanderPoint = { 0, 0 };
	float wanderRadius = 50.0f;

	// Flocking
	bool isFlockLeader = false;
	float separationDistance = 50.0f;
	float cohesionDistance = 100.0f;
	float viewAngle = 60.0f;
	float alignDistance = 50.0f;
	float threshold = 10.0f;

	float separationWeight = 2.0f;
	float cohesionWeight = 1.5f;
	float seekWeight = 1.25f;
};