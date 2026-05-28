#pragma once
#include "raylib.h"
//#include "raymath.h"
//#include "resource_dir.h"
#include <vector>
#include "Body.h"
#include "Random.h"
#include "Collision.h"


class World {
public:
	World()
	{
		bodies.reserve(1000);
	}


	static Vector2 gravity;
	std::vector<Body> bodies;
	std::vector<class Effector*> effectors;

	static void SetGravity(Vector2 newgravity) { gravity = newgravity; }
	void SetBounds(Vector2 min, Vector2 max) { boundsMin = min; boundsMax = max; }

	std::vector<Body>& GetBodies() { return bodies; }
	const std::vector<Body>& GetBodies() const { return bodies; }

	Body* GetBodyIntersect(Vector2& position);

	//staic void SetGravity(Vector2 newgravity) 
	//void SetBounds

	void UpdateCollision();
	void Step(float dt);
	void Draw();
	void AddBody(const Body& body);
	void AddEffector(class Effector* effector);
	std::vector<class Spring*> springs;
	std::vector<Contact> contacts;
	Vector2 boundsMin{ -10.0f, -5.0f };
	Vector2 boundsMax{ 10.0f,  5.0f };
};
