#include "World.h"
#include "Effector.h"

Vector2 World::gravity = { 0, 9.81f };

void World::Step(float dt)
{
	/*
	for (auto& body : bodies) body.acceleration = Vector2{ 0,0 };
	for (auto& body : bodies) body.AddForce(gravity * body.gravityScale * 100.0f, ForceMode::Acceleration);

	//for (auto& body : bodies) body.acceleration = gravity * 100.0f;
	//for (auto& body : bodies) body.AddForce(gravity * 100.0f);

	for (auto& effector : effectors) effector->Apply(bodies);

	for (auto& spring : springs) spring->Apply(100.0f);

	for (int i = 0; i < 4; i++) UpdateCollision();

	for (auto& body : bodies) body.acceleration = Vector2{ 0,0 };
	*/

	// update gravity
	for (auto& body : bodies) body.AddForce(gravity * body.gravityScale, ForceMode::Acceleration);

	// force effector
	for (auto& effector : effectors) effector->Apply(bodies);

	// spring
	//for (auto& spring : springs) spring->Apply(100.0f);

	// integrator
	//for (auto& body : bodies) if (body.bodyType == BodyType::Dynamic) SemiImplicitEuler(body, dt);
	for (auto& body : bodies) if (body.bodyType == BodyType::Dynamic) body.Step(dt);
	for (int i = 0; i < 4; i++) UpdateCollision();

	// reset acceleration
	for (auto& body : bodies) body.acceleration = Vector2{ 0, 0 };
	}

//void World::AddSpring(Body& bodyA, Body& bodyB, float restLength, 
//Spring* spring = new Spring(

Body* World::GetBodyIntersect(Vector2& position)
{
	for (auto& body : bodies) 
	{
		if (CheckCollisionPointCircle(position, body.position, body.size)) 
		{
			return &body;
		}
	}

	return nullptr;
}

void World::UpdateCollision() {
	contacts.clear();
	CreateContacts(bodies, contacts);
	SeparateContacts(contacts);
	ResolveContacts(contacts);

	for (auto& body : bodies) {
		if (body.position.x + body.size > boundsMax.x)
		{
			body.position.x = GetScreenWidth() - body.size;
			body.velocity.x *= -body.restitution;
		}
		if (body.position.x - body.size < boundsMin.x)
		{
			body.position.x = body.size;
			body.velocity.x *= -body.restitution;
		}

		if (body.position.y + body.size > boundsMax.y)
		{
			body.position.y = GetScreenHeight() - body.size;
			body.velocity.y *= -body.restitution;
		}
		if (body.position.y - body.size < boundsMin.y)
		{
			body.position.y = body.size;
			body.velocity.y *= -body.restitution;
		}
	}
}


void World::Draw()
{
	for (auto& effector : effectors) effector->Draw();
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
