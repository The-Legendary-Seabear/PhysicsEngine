#pragma once
#include "raylib.h"
//#include "raymath.h"
//#include "resource_dir.h"
#include <vector>
#include "Body.h"
#include "Random.h"

class World {
public:
	Vector2 gravity{ 0, 9.81f };
	std::vector<Body> bodies;

	void Step(float dt);
	void Draw();
	void AddBody(const Body& body);
};
