#pragma once
#include "resource_dir.h"

class Random {
public:
	inline float GetRandomFloat(float max) {
		return GetRandomValue(0, max);
	}

	inline float GetRandomFloat(float min, float max) {
		return GetRandomValue(min, max);
	}

	inline float GetRandomFloat() {
		return GetRandomValue(0, 10000) / (float)10000;
	}
};
