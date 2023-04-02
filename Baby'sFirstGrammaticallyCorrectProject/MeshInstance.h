#pragma once

#include "Maths.h"
#include "Material.h"

class MeshRenderData;

class MeshInstance
{
	MeshRenderData* meshData = nullptr;

public:
	Vec3 position;
	Vec4 axisAngleRotation;
	Vec3 scale;
	Material material;

	void Bind();
	void Draw();
	void Unbind();

	MeshInstance& SetMaterial(Material material);
	MeshInstance& SetMeshData(MeshRenderData* data);

	Mat4 LocalToWorld() const;
};