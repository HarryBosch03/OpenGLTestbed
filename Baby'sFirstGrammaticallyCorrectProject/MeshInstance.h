#pragma once

#include "Maths.h"

class MeshRenderData;

class MeshInstance
{
public:
	Vec3 position;
	Vec4 axisAngleRotation;
	Vec3 scale;

	MeshRenderData* meshData = nullptr;

	void Bind();
	void Draw();
	void Unbind();

	Mat4 LocalToWorld() const;
};