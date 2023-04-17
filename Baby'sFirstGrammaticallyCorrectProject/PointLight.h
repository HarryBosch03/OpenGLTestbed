#pragma once

#include "Maths.h"
#include "SceneObject.h"

#include <vector>

class PointLight : public SceneObject
{
public:
	PointLight(Vec3 position = Zero, Vec3 color = One, float brightness = 1.0f);

	Vec3 position;
	Vec3 color;
	float brightness;

	inline const Vec3 FinalColor() { return color * brightness; }
};