#include "MeshInstance.h"

#include "ShaderProgram.h"
#include "Camera.h"
#include "Texture.h"
#include "Mesh.h"
#include "InspectorUtility.h"
#include "AssetDatabase.h"
#include "LogMaster.h"

#include "imgui/imgui.h"

using namespace Utility::Inspector;

MeshInstance::MeshInstance()
{
	SetTypename("Mesh Instance");
}

void MeshInstance::DrawGUIContent()
{
	if (ImGui::CollapsingHeader(("Transform##MeshInstance." + std::to_string(id)).c_str()))
	{
		ImGui::InputFloat3(AppendID("Position"), &position[0]);
		ImGui::InputFloat4(AppendID("Rotation (Angle Axis)"), &axisAngleRotation[0]);
		ImGui::InputFloat3(AppendID("Scale"), &scale[0] );
	}

	for (int i = 0; i < materials.size(); i++)
	{
		if (ImGui::CollapsingHeader(("Material." + std::to_string(i) + "##MeshInstance." + std::to_string(id)).c_str()))
		{
			Material& mat = materials[i];
			Utility::Inspector::DrawMaterial(mat);
		}
	}

	Utility::Inspector::AssetDropdown(mesh, "Mesh");

	ImGui::SameLine();
	ImGui::Text("Mesh File Name");
}

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

	if (mesh)
	{
		std::strcpy(meshFileName, mesh->FileName().c_str());
	}

	return *this;
}