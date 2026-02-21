#include "Boid.h"
#include <ofMain.h>

Boid::Boid()
{
	this->destinations[0] = { 10, 10 };		// Top left
	this->destinations[1] = { 994, 10 };		// Top right
	this->destinations[2] = { 994, 758 };		// Bottom right
	this->destinations[3] = { 10, 768 };		// Original position

	this->currentDestination = 0;

	this->rb.position = { 400, 400 };

	float phi = rand() * (2.0 * PI) / RAND_MAX;
	float x = cos(phi);
	float y = sin(phi);
	ofVec2f randomDirection = { x, y };
	this->rb.velocity = randomDirection;

	this->rb.velocity *= ofRandom(50, 100);

	this->rb.maxSpeed = 100.0f;
}

Boid::Boid(std::vector<Boid*> _flockmates)
{
	this->rb.position = { 250, 250 };
	this->rb.velocity = { 1, 1 };

	this->rb.maxSpeed = 100.0f;
}

void Boid::Update(float dt, SteeringOutput steering, std::vector<Boid*> boids)
{
	if (this->movementIsKinematic) {
		if (this->currentDestination <= 3)
		{
			if (!HasReachedDestination())
			{
				this->rb.KinematicUpdate(dt);
			}
			else
			{
				// Set new destination
				this->currentDestination++;
				this->rb.velocity = (this->destinations[currentDestination] - this->destinations[currentDestination - 1]).normalize();
				this->rb.velocity.x = round(this->rb.velocity.x);
				this->rb.velocity.y = round(this->rb.velocity.y);
			}
		}
	}
	else
	{
		if (this->behaviorType == SEEK)
			steering = DynamicSeek();
		else if (this->behaviorType == ARRIVE)
			steering = DynamicArrive();
		else if (this->behaviorType == ARRIVE2)
			steering = ArriveV2();
		else if (this->behaviorType == WANDER)
			steering = Wander();
		else if (this->behaviorType == WANDER2)
			steering = WanderV2();
		else if (this->behaviorType == FLOCK)
			steering = Flock(boids);

		this->rb.DynamicUpdate(dt, steering);

		// Screen wrapping
		if (this->rb.position.x < 0)
		{
			this->rb.position.x = 1024;
		}
		else if (this->rb.position.x > 1024)
		{
			this->rb.position.x = 0;
		}
		if (this->rb.position.y < 0)
		{
			this->rb.position.y = 768;
		}
		else if (this->rb.position.y > 768)
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

	if (this->isFlockLeader)
		ofSetColor(255, 0, 0);
	else
		ofSetColor(255, 255, 255);

	ofDrawCircle(
		ofVec2f::zero(),
		10
	);

	ofDrawTriangle(0, 10, 0, -10, 20, 0);

	ofPopMatrix();
}

SteeringOutput Boid::DynamicSeek()
{
	SteeringOutput steering;

	ofVec2f direction = this->target - this->rb.position;
	steering.linear = direction;
	steering.linear.normalize();
	steering.linear *= steering.maxAcceleration;
	//steering.angular = 0;

	return steering;
}

SteeringOutput Boid::DynamicArrive()
{
	SteeringOutput steering;
	ofVec2f direction = this->target - this->rb.position;
	float distance = direction.length();
	
	if (distance < targetRadius)
	{
		rb.velocity = ofVec2f::zero();
		return steering;
	}

	float targetSpeed;
	if (distance > slowRadius)
		targetSpeed = steering.maxAcceleration;
	else
		targetSpeed = steering.maxAcceleration * (distance / slowRadius);

	ofVec2f targetVelocity = direction;
	targetVelocity.normalize();
	targetVelocity *= targetSpeed;
	steering.linear = targetVelocity - rb.velocity;

	if (steering.linear.length() > steering.maxAcceleration)
	{
		steering.linear.normalize();
		steering.linear *= steering.maxAcceleration;
	}

	steering.angular = 0;
	return steering;
}

SteeringOutput Boid::ArriveV2()
{
	SteeringOutput steering;
	ofVec2f direction = this->target - this->rb.position;
	float distance = direction.length();

	if (distance < targetRadius)
	{
		rb.velocity = ofVec2f::zero();
		return steering;
	}

	float targetSpeed = steering.maxAcceleration;

	ofVec2f targetVelocity = direction;
	targetVelocity.normalize();
	targetVelocity *= targetSpeed;
	steering.linear = targetVelocity - rb.velocity;

	if (steering.linear.length() > steering.maxAcceleration)
	{
		steering.linear.normalize();
		steering.linear *= steering.maxAcceleration;
	}

	return steering;
}

SteeringOutput Boid::Wander()
{
	SteeringOutput steering;

	// Get random 2d unit vector in circle
	float phi = rand() * (2.0 * PI) / RAND_MAX;

	float x = cos(phi);
	float y = sin(phi);

	ofVec2f randomDirection = { x, y };
	steering.linear = randomDirection;

	steering.linear *= maxAcceleration;

	return steering;
}

SteeringOutput Boid::WanderV2()
{
	SteeringOutput steering;

	wanderPoint = this->rb.velocity;
	wanderPoint.normalize();
	wanderPoint *= 100.0f;
	wanderPoint += this->rb.position;

	float theta = wanderAngle + atan2(this->rb.velocity.y, this->rb.velocity.x);

	float x = wanderRadius * cos(theta);
	float y = wanderRadius * sin(theta);

	wanderPoint += { x, y };

	steering.linear = wanderPoint - this->rb.position;
	steering.linear.normalize();
	steering.linear *= this->maxAcceleration;

	this->wanderAngle += ofRandom(-0.3, 0.3);

	return steering;
}

SteeringOutput Boid::Flock(std::vector<Boid*> boids)
{
	SteeringOutput steering;

	ofVec2f alignment = Alignment(boids).linear;
	ofVec2f cohesion = Cohesion(boids).linear;
	ofVec2f separation = Separation(boids).linear;

	alignment *= seekWeight;
	cohesion *= cohesionWeight;
	separation *= separationWeight;

	steering.linear += alignment;
	steering.linear += cohesion;
	steering.linear += separation;

	return steering;
}

SteeringOutput Boid::Alignment(std::vector<Boid*> boids)
{
	SteeringOutput steering;
	for (Boid* boid : boids)
	{
		if (boid != this && boid->isFlockLeader)
		{
			this->target = boid->rb.position;
		}
	}
	return DynamicSeek();
}

SteeringOutput Boid::Separation(std::vector<Boid*> boids)
{
	SteeringOutput steering;

	int count = 0;
	for (Boid* boid : boids)
	{
		if (boid != this)
		{
			ofVec2f direction = boid->rb.position - this->rb.position;
			float distance = direction.length();
			if (distance < separationDistance)
			{
				ofVec2f difference = this->rb.position - boid->rb.position;
				difference /= (distance * distance);

				steering.linear += difference;
				count++;
			}
		}
	}

	if (count > 0)
	{
		steering.linear /= count;
		steering.linear.normalize();
		steering.linear *= steering.maxAcceleration;
		steering.linear -= this->rb.velocity;
	}

	return steering;
}

SteeringOutput Boid::Cohesion(std::vector<Boid*> boids)
{
	SteeringOutput steering;

	int count = 0;
	for (Boid* boid : boids)
	{
		if (boid != this)
		{
			ofVec2f targetDirection = boid->rb.position - this->rb.position;
			float distance = targetDirection.length();
			if (distance < cohesionDistance)
			{
				steering.linear += boid->rb.position;
				count++;
			}
		}
	}
	if (count > 0)
	{
		steering.linear /= count;
		steering.linear -= this->rb.position;
		steering.linear.normalize();
		steering.linear *= steering.maxAcceleration;
		steering.linear -= this->rb.velocity;
	}

	return steering;
}

bool Boid::HasReachedDestination()
{
	return rb.position.distance(destinations[currentDestination]) <= 0.5;
}
