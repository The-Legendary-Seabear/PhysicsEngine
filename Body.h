#pragma once
#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <vector>


struct Body
{
public:
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	float mass;
	float size;
	float restitution;


	void AddForce(Vector2 force);
	void Step(float dt);
	void Draw();

};

