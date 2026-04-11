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


	Vector2 gravity{ 0, 9.81f };
	std::vector<Body> bodies;
	std::vector<class Effector*> effectors;

	void Step(float dt);
	void Draw();
	void AddBody(const Body& body);
	void AddEffector(class Effector* effector);
	std::vector<Contact> contacts;
};
