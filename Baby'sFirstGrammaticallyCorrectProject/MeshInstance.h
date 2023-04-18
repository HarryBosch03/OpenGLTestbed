#pragma once

#include "Maths.h"
#include "Material.h"
#include "SceneObject.h"

#include <vector>

class Mesh;

class MeshInstance : public SceneObject
{
	Mesh* mesh = nullptr;

public:
	Vec3 position = Zero;
	Vec4 axisAngleRotation = Up;
	Vec3 scale = One;

	std::vector<Material> materials;

	MeshInstance();

	void DrawGUIContent() override;
	void Draw() override;

	MeshInstance& SetMaterials(const Material& material);
	MeshInstance& SetMaterial(int index, const Material& material);

	MeshInstance& SetMeshData(Mesh* mesh);

	Mat4 LocalToWorld() const;

	static const std::vector<MeshInstance*>& All();
};