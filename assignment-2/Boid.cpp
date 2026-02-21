#include "Boid.h"
#include <ofMain.h>

Boid::Boid()
{
	this->destinations[0] = { 10, 10 };		// Top left
	this->destinations[1] = { 994, 10 };		// Top right
	this->destinations[2] = { 994, 758 };		// Bottom right
	this->destinations[3] = { 10, 768 };		// Original position

	this->current_destination = 0;

	this->target = { 200.0f, 200.0f };

	this->rb.position = { 400, 400 };

	float phi = rand() * (2.0 * PI) / RAND_MAX;
	float x = cos(phi);
	float y = sin(phi);
	ofVec2f random_direction = { x, y };
	this->rb.velocity = random_direction;

	this->rb.velocity *= ofRandom(50, 100);

	this->rb.maxSpeed = 100.0f;
}

Boid::Boid(std::vector<Boid*> flockmates)
{
	this->rb.position = { 250, 250 };
	this->rb.velocity = { 1, 1 };

	this->rb.maxSpeed = 100.0f;
}

void Boid::Update(float dt, SteeringOutput steering, std::vector<Boid*> boids)
{
	if (this->movement_is_kinematic) {
		if (this->current_destination <= 3)
		{
			if (!HasReachedDestination())
			{
				this->rb.KinematicUpdate(dt);
			}
			else
			{
				// Set new destination
				this->current_destination++;
				this->rb.velocity = (this->destinations[current_destination] - this->destinations[current_destination - 1]).normalize();
				this->rb.velocity.x = round(this->rb.velocity.x);
				this->rb.velocity.y = round(this->rb.velocity.y);
			}
		}
	}
	else
	{
		steering = DynamicArrive();

		this->rb.DynamicUpdate(dt, steering);

		// Screen wrapping
		if (this->rb.position.x < 0)
		{
			this->rb.position.x = 1920;
		}
		else if (this->rb.position.x > 1920)
		{
			this->rb.position.x = 0;
		}
		if (this->rb.position.y < 0)
		{
			this->rb.position.y = 1080;
		}
		else if (this->rb.position.y > 1080)
		{
			this->rb.position.y = 0;
		}
	}
}

void Boid::Draw()
{
	ofPushMatrix();

	ofTranslate(this->rb.position.x, this->rb.position.y, 0);
	float angle = atan2(rb.velocity.y, rb.velocity.x);
	ofRotateZRad(angle);

	ofSetColor(255, 255, 255);

	ofDrawCircle(
		ofVec2f::zero(),
		10
	);

	ofDrawTriangle(0, 10, 0, -10, 20, 0);

	ofPopMatrix();
}

SteeringOutput Boid::DynamicArrive()
{
	SteeringOutput steering;
	ofVec2f direction = this->target - this->rb.position;
	float distance = direction.length();

	if (distance < target_radius)
	{
		if (this->current_path_index < current_path.size() - 1 && is_currently_pathing)
		{
			this->current_path_index++;
			this->target = { (float)current_path[current_path_index].x, (float)current_path[current_path_index].y };
		}
		else
		{
			rb.velocity = ofVec2f::zero();
		}
		
		return steering;
	}

	float targetSpeed = steering.max_acceleration;

	ofVec2f target_velocity = direction;
	target_velocity.normalize();
	target_velocity *= targetSpeed;
	steering.linear = target_velocity - rb.velocity;

	if (steering.linear.length() > steering.max_acceleration)
	{
		steering.linear.normalize();
		steering.linear *= steering.max_acceleration;
	}

	return steering;
}

bool Boid::HasReachedDestination()
{
	return rb.position.distance(destinations[current_destination]) <= 0.5;
}
