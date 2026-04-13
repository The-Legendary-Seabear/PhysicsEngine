#include "Collision.h"

Random random;

void CreateContacts(std::vector<Body>& bodies, std::vector<Contact>& contacts)
{
	for (int i = 0; i < bodies.size(); i++)
	{
		for (int j = i + 1; j < bodies.size(); j++)
		{
			Body& bodyA = bodies[i];
			Body& bodyB = bodies[j];

			if (bodyA.bodyType != BodyType::Dynamic && bodyB.bodyType != BodyType::Dynamic) continue;//< bodyA is not dynamic and bodyB is not dynamic) continue;

			if (Intersects(bodyA, bodyB))
			{
				Contact contact;
				contact.bodyA = &bodyA;//<get address of bodyA>;
				contact.bodyB = &bodyB;//<get address of bodyB>;

				Vector2 direction = bodyA.position - bodyB.position;//<direction vector from bodyB to bodyA>;
				float distanceSqr = Vector2LengthSqr(direction);//<length of direction vector>

					if (distanceSqr <= EPSILON) {
						direction = Vector2{ random.GetRandomFloat(-0.05f, 0.05f), random.GetRandomFloat(-0.05f, 0.05f) };
						distanceSqr = Vector2LengthSqr(direction);
					}

					float distance = sqrtf(distanceSqr);
					float radius = bodyA.size + bodyB.size;//<size of bodyA + size of bodyB>;
					contact.depth = radius - distance;//<how far do the bodies intersect ? >
					contact.normal = Vector2Normalize(direction);//<normalized direction vector>
					contact.restitution = (bodyA.restitution + bodyB.restitution) * 0.5f;

					//<add contact to contacts>
					contacts.push_back(contact);

			}
		}
	}
}

void SeparateContacts(std::vector<Contact>& contacts)
{
	for (auto& contact : contacts)
	{
		float totalInverseMass = contact.bodyA->inverseMass + contact.bodyB->inverseMass;//<add bodyA inverse mass + bodyB inverse mass>
		Vector2 separation = contact.normal * (contact.depth / totalInverseMass);//(<divide the contact depth by the total inverse mass>);
		contact.bodyA->position = contact.bodyA->position + (separation * contact.bodyA->inverseMass);//(<separation * bodyA inverse mass>);
		contact.bodyB->position = contact.bodyB->position - (separation * contact.bodyB->inverseMass);//(<separation * bodyB inverse mass>);
	}
}

bool Intersects(const Body& bodyA, const Body& bodyB)
{
	float distance = Vector2Distance(bodyB.position, bodyA.position);//<distance between bodyA and body B>
	float radius = bodyA.size + bodyB.size;//<size of bodyA + size of bodyB>;

	return distance <= radius;//(<colliding if distance is <= to radius>);
}

void ResolveContacts(std::vector<Contact>& contacts)
{
	for (auto& contact : contacts)
	{
		// compute relative velocity
		Vector2 rv = contact.bodyA->velocity - contact.bodyB->velocity;
		// project relative velocity onto the contact normal
		float nv = Vector2DotProduct(rv, contact.normal);

		// skip if bodies are separating
		if (nv > 0) continue;

		// total inverse mass = (1/mA + 1/mB)
		float totalInverseMass = contact.bodyA->inverseMass + contact.bodyB->inverseMass;//<bodyA inverse mass + bodyB inverse mass>;
		// impulse scalar = -(1 + restitution) * vn / (1/mA + 1/mB)
		float impulseMagnitude = -(1 + contact.restitution) * nv / totalInverseMass;

		// impulse vector along contact normal
		Vector2 impulse = Vector2Scale(contact.normal, impulseMagnitude); //<scale contact normal by impulse magnitude>

			// apply equal and opposite impulses
		contact.bodyA->AddForce(impulse, ForceMode::Impluse);//(<+impulse with impulse force mode>);
		contact.bodyB->AddForce(impulse * -1, ForceMode::Impluse);//(<-impulse with impulse force mode>);
	}
}
