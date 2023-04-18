#include "MeshInstance.h"

#include "ShaderProgram.h"
#include "Camera.h"
#include "Texture.h"
#include "Mesh.h"

#include "imgui.h"

MeshInstance::MeshInstance()
{
	SetTypename("Mesh Instance");
}

void MeshInstance::DrawGUIContent()
{
	for (int i = 0; i < materials.size(); i++)
	{
		if (ImGui::CollapsingHeader(("Material." + std::to_string(i) + "##MeshInstance." + std::to_string(id)).c_str()))
		{
			std::string append = "##Material." + std::to_string(id) + "." + std::to_string(i);

			ImGui::ColorEdit4(("Color" + append).c_str(), &materials[i].properties.color[0]);
			ImGui::SliderFloat2(("Metalness" + append).c_str(), &materials[i].properties.metalness[0], 0.0f, 1.0f);
			ImGui::SliderFloat2(("Roughness" + append).c_str(), &materials[i].properties.roughness[0], 0.0f, 1.0f);
			ImGui::SliderFloat(("Normal" + append).c_str(), &materials[i].properties.normal, 0.0f, 1.0f);
			ImGui::SliderFloat(("Height" + append).c_str(), &materials[i].properties.height, 0.0f, 1.0f);
			ImGui::SliderFloat(("Ambient Occlusion" + append).c_str(), &materials[i].properties.ao, 0.0f, 1.0f);
		}
	}
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
	return *this;
}