#pragma once

#include "Maths.h"

struct DirectionalLightData
{
	Vec3 direction;
	Vec3 color;

	DirectionalLightData() = default;
	inline DirectionalLightData(Vec3 direction, Vec3 color)
	{
		this->direction = direction;
		this->color = color;
	}
};
