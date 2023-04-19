#include "MeshInstance.h"

#include "ShaderProgram.h"
#include "Camera.h"
#include "Texture.h"
#include "Mesh.h"
#include "InspectorUtility.h"
#include "AssetDatabase.h"
#include "LogMaster.h"

#include "imgui.h"

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
			ImGui::Indent();
			std::string append = "##Material." + std::to_string(id) + "." + std::to_string(i);

			Material& mat = materials[i];

			int count, maxNameLength;
			glGetProgramInterfaceiv(mat.Shader()->programHandle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &count);
			glGetProgramInterfaceiv(mat.Shader()->programHandle, GL_UNIFORM, GL_MAX_NAME_LENGTH, &maxNameLength);

			std::vector < std::string> textures;
			GLchar* buffer = new GLchar[maxNameLength];
			const GLenum properties = GL_TYPE;
			for (int i = 0; i < count; i++)
			{
				GLint type;
				glGetProgramResourceiv(mat.Shader()->programHandle, GL_UNIFORM, i, 1, &properties, count, nullptr, &type);
				if (type != GL_SAMPLER_2D) continue;
				glGetProgramResourceName(mat.Shader()->programHandle, GL_UNIFORM, i, maxNameLength, nullptr, buffer);
				textures.push_back(buffer);
			}
			delete[] buffer;

			for (const std::string& texture : textures)
			{
				Utility::Inspector::AssetDropdown<Texture>(mat.Textures(texture), texture, "tex" + texture);
			}

			ImGui::ColorEdit4(AppendID("Color" + append), &mat.properties.color[0]);
			ImGui::SliderFloat2(AppendID("Metalness" + append), &mat.properties.metalness[0], 0.0f, 1.0f);
			ImGui::SliderFloat2(AppendID("Roughness" + append), &mat.properties.roughness[0], 0.0f, 1.0f);
			ImGui::SliderFloat(AppendID("Normal" + append), &mat.properties.normal, 0.0f, 1.0f);
			ImGui::SliderFloat(AppendID("Height" + append), &mat.properties.height, 0.0f, 1.0f);
			ImGui::SliderFloat(AppendID("Ambient Occlusion" + append), &mat.properties.ao, 0.0f, 1.0f);
			ImGui::Unindent();
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