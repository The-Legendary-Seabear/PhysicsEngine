#pragma once
#include "Body.h"
#include <vector>

class Effector 
{
public:
	virtual void Apply(std::vector<Body>& bodies) = 0;
	virtual void Draw();

	void CollectBodiesInside(std::vector<Body>& ibodies, std::vector<Body*>& obodies);

	Effector(Vector2 position, float size) : position{ position }, size{ size } {}



protected:
	Vector2 position;
	float size;
};

enum class EffectorType
{
	Gravitation,
	Point,
	Area,
	Drag
};
