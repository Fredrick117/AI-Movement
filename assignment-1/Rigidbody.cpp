#include "Rigidbody.h"
#include <ofLog.h>
#include <ofMain.h>

void Rigidbody::KinematicUpdate(float dt)
{
	position += velocity;
}

void Rigidbody::DynamicUpdate(float dt, SteeringOutput steering)
{
	velocity += steering.linear * dt;
	position += velocity * dt;

	// This causes issues with facing the direction of movement...why?
	/*rotation += steering.angular * dt;
	orientation += rotation * dt;*/

	if (this->id == 2)
		ofLog(OF_LOG_NOTICE, "VELOCITY: " + ofToString(this->velocity));

	// Clamp to max speed
	if (velocity.length() > maxSpeed)
	{
		velocity.normalize();
		velocity *= maxSpeed;
	}
}
