#pragma once
#include <ofVec2f.h>
#include "SteeringOutput.h"

struct Rigidbody
{
	// DEBUG
	int id;

	ofVec2f position;
	ofVec2f velocity;
	float orientation;
	float rotation;
	float maxSpeed = 400.0f;
	float angularSpeed;

	void KinematicUpdate(float dt);
	void DynamicUpdate(float dt, SteeringOutput steering);
};