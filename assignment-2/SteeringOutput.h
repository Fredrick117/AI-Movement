#pragma once

#include <ofVec2f.h>

struct SteeringOutput
{
	SteeringOutput()
	{
		linear = { 0, 0 };
		angular = 0;
	}

	ofVec2f linear;
	float angular;

	float max_acceleration = 1000.0f;
};