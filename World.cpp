#include "World.h"

void World::Step(float dt) 
{
	for (auto& body : bodies) body.acceleration = Vector2{ 0, 0 };
	for (auto& body : bodies) body.AddForce(gravity * 100.0f);

	for (auto& body : bodies) body.Step(dt);

	for (auto& body : bodies) {
		if (body.position.x + body.size > GetScreenWidth()) {
			body.position.x = GetScreenWidth() - body.size;
			body.velocity.x *= -body.restitution;
		}
		if (body.position.x - body.size <= 0) {
			body.position.x = body.size;
			body.velocity.x *= -body.restitution;
		}

		if (body.position.y + body.size > GetScreenHeight()) {
			body.position.y = GetScreenHeight() - body.size;
			body.velocity.y *= -body.restitution;
		}
		if (body.position.y - body.size <= 0) {
			body.position.y = GetScreenHeight() + body.size;
			body.velocity.y *= -body.restitution;
		}
	}
}
void World::Draw()
{
	for (auto& body : bodies) body.Draw();
}

void World::AddBody(const Body& body)
{
	bodies.push_back(body);
}