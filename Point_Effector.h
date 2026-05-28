#pragma once
#include "Effector.h"

class PointEffector : public Effector 
{
public:
	PointEffector(Vector2 posiiton, float size, float forceMagnitude) :
		Effector{ posiiton, size },
		forceMagnitude{forceMagnitude}
	{}

	void Apply(std::vector<Body>& bodies) override;
	void Draw() override;

private:
	
	float forceMagnitude;
};
