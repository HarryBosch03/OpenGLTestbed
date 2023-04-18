#pragma once

#include "Maths.h"
#include "SceneObject.h"

#include <vector>

class DirectionalLight : public SceneObject
{
public:
	DirectionalLight(Vec3 direction = { 0.0, -1.0, 0.0 }, Vec3 color = One, float brightness = 1.0);

	Vec3 direction;
	Vec3 color;
	float brightness;

	void DrawGUIContent() override;

	inline Vec3 FinalColor() { return color * brightness; }
};