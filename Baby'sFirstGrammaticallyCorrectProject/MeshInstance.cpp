#include "MeshInstance.h"

#include "MeshRenderData.h"
#include "ShaderProgram.h"
#include "Camera.h"

void MeshInstance::Bind()
{
	if (meshData)
	{
		meshData->Bind();
	}
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
	if (meshData)
	{
		meshData->Unbind();
	}
}

Mat4 MeshInstance::LocalToWorld() const
{
	return TRS(position, axisAngleRotation, scale);
}