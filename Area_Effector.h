#pragma once
#include "Effector.h"
class AreaEffector : public Effector
{
	public:
		AreaEffector(Vector2 posiiton, float size, float angle, float forceMagnitude) :
			Effector{ posiiton, size },
			angle{angle},
			forceMagnitude{ forceMagnitude }
		{
		}

		void Apply(std::vector<Body>& bodies) override;
		void Draw() override;

	private:
		float angle;
		float forceMagnitude;
	
};

