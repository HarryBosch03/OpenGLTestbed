#include "MeshInstance.h"

#include "MeshRenderData.h"
#include "ShaderProgram.h"
#include "Camera.h"

void MeshInstance::Bind()
{
	material.Bind();
	if (meshData) meshData->Bind();
}

void MeshInstance::Draw()
{
	if (meshData)
	{
		Mat4 m = LocalToWorld();
		ShaderProgram::Current->SetModelMatrix(m);
		meshData->Draw();
	}
}

void MeshInstance::Unbind()
{
	if (meshData) meshData->Unbind();
	material.Unbind();
}

MeshInstance& MeshInstance::SetMaterial(Material material)
{
	this->material = material;
	return *this;
}

MeshInstance& MeshInstance::SetMeshData(MeshRenderData* data)
{
	this->meshData = data;
	return *this;
}

Mat4 MeshInstance::LocalToWorld() const
{
	return TRS(position, axisAngleRotation, scale);
}