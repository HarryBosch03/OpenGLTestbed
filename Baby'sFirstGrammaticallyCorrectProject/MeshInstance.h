#pragma once

#include "Maths.h"
#include "Material.h"

class MeshRenderData;

class MeshInstance
{
	MeshRenderData* meshData = nullptr;
	void Bind();
	void Unbind();

public:
	Vec3 position = {};
	Vec4 axisAngleRotation = {};
	Vec3 scale = {};
	Material material;

	void Draw();

	MeshInstance& SetMaterial(Material material);
	MeshInstance& SetMeshData(MeshRenderData* data);

	Mat4 LocalToWorld() const;
};