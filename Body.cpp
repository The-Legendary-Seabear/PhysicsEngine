#include "Body.h"

void Body::AddForce(Vector2 force)
{
	acceleration += force / mass;
}

void Body::Step(float dt)
{
	velocity += acceleration * dt;
	position += velocity * dt;

	acceleration = Vector2{ 0, 0 };
}

void Body::Draw()
{
	DrawCircleV(position, size, RED);
}
