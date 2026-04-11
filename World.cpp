#include "World.h"
#include "Effector.h"


void World::Step(float dt) 
{
	for (auto& body : bodies) body.acceleration = Vector2{ 0,0 };
	for (auto& body : bodies) body.AddForce(gravity * body.gravityScale * 100.0f, ForceMode::Acceleration);

	//for (auto& body : bodies) body.acceleration = gravity * 100.0f;
	//for (auto& body : bodies) body.AddForce(gravity * 100.0f);

	for (auto& effector : effectors) effector->Apply(bodies);

	for (auto& body : bodies) if(body.bodyType == BodyType::Dynamic) body.Step(dt);

	contacts.clear();
	CreateContacts(bodies, contacts);
	SeparateContacts(contacts);

	for (auto& body : bodies) {
		if (body.position.x + body.size > GetScreenWidth()) 
		{
			body.position.x = GetScreenWidth() - body.size;
			body.velocity.x *= -body.restitution;
		}
		if (body.position.x - body.size < 0) 
		{
			body.position.x = body.size;
			body.velocity.x *= -body.restitution;
		}

		if (body.position.y + body.size > GetScreenHeight()) 
		{
			body.position.y = GetScreenHeight() - body.size;
			body.velocity.y *= -body.restitution;
		}
		if (body.position.y - body.size < 0) 
		{
			body.position.y = body.size;
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

void World::AddEffector(Effector* effector)
{
	effectors.push_back(effector);
}
