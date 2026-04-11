#include "Point_Effector.h"

void PointEffector::Apply(std::vector<Body>& bodies)
{
	for (auto& body : bodies) 
	{

	}
}

void PointEffector::Draw() 
{
	DrawCircleLinesV(position, size, WHITE);
}
