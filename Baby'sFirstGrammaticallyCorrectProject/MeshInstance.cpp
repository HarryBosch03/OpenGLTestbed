#include "MeshInstance.h"

#include "ShaderProgram.h"
#include "Camera.h"
#include "Texture.h"
#include "Mesh.h"

void MeshInstance::Draw()
{
	if (!mesh) return;

	Mat4 m = LocalToWorld();
	ShaderProgram::Current->SetModelMatrix(m);
	mesh->Draw(materials);
}

Mat4 MeshInstance::LocalToWorld() const
{
	return TRS(position, axisAngleRotation, scale);
}

MeshInstance& MeshInstance::SetMaterials(const Material& material)
{
	if (!mesh) return *this;

	materials.clear();
	materials.reserve(mesh->submeshes.size());

	for (int i = 0; i < mesh->submeshes.size(); i++)
	{
		materials.push_back(material);
	}
	return *this;
}

MeshInstance& MeshInstance::SetMaterial(int index, const Material& material)
{
	materials[index] = material;
	return *this;
}

MeshInstance& MeshInstance::SetMeshData(Mesh* mesh)
{
	this->mesh = mesh;
	return *this;
}