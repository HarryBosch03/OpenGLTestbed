#include "InspectorUtility.h"

#include "Material.h"
#include "ShaderProgram.h"
#include "UniformBufferObject.h"
#include "StringUtility.h"

#include "imgui/imgui.h"

int workingID = -1;

int& Utility::Inspector::WorkingID() { return workingID; }

std::string buffer;

const char* Utility::Inspector::AppendID(const std::string& text)
{
	buffer = text + "##" + std::to_string(workingID);
	return buffer.c_str();
}

void Utility::Inspector::DrawMaterial(Material& material)
{
	std::string programFilename = material.Shader()->FileName();
	std::string name = programFilename.substr(programFilename.rfind('/'));

	if (!ImGui::CollapsingHeader((name + "##material").c_str())) return;
	return;
	
	ImGui::Indent();

	int rawSize;
	std::vector<byte> raw;
	byte* ubo = UniformBufferObject::SoftGet("properties", &rawSize);

	for (const ShaderProperty& prop : material.Shader()->Properties())
	{
		const std::string& tn = prop.Typename();
		const std::string labelString = prop.Name() + "##MaterialInspector." + material.Shader()->FileName() + "." + prop.Ref();
		const char* label = labelString.c_str();

		if (tn == "tex2D")
		{
			AssetDropdown(material.Textures(prop.Ref()), prop.Name(), material.Shader()->FileName() + prop.Ref());
		}
		else if (tn == "color")
		{
			//Vec4& val = UniformBufferObject::Lookup<Vec4>(prop.Ref(), Utility::String::FromVec(prop.Default()));
			Vec4 val;
			ImGui::ColorEdit4(label, &val[0]);
			for (int i = 0; i < sizeof(Vec4); i++)
			{
				raw.push_back(((byte*)&val.x)[i]);
			}
		}
		else if (tn == "remap")
		{
			const float l = std::stof(prop.Args()[0]), u = std::stof(prop.Args()[1]);
			Vec2 val;
			ImGui::SliderFloat2(label, &val[0], l, u);
			for (int i = 0; i < sizeof(Vec2); i++)
			{
				raw.push_back(((byte*)&val.x)[i]);
			}
		}
		else if (tn == "range")
		{
			const float l = std::stof(prop.Args()[0]), u = std::stof(prop.Args()[1]);
			float val;
			ImGui::SliderFloat(label, &val, l, u);
			for (int i = 0; i < sizeof(float); i++)
			{
				raw.push_back(((byte*)&val)[i]);
			}
		}
		else
		{
			ImGui::Text(("Property Type \"" + tn + "\" is not supported in \"" + prop.Name() + "\"").c_str());
		}
	}

	std::memcpy(ubo, raw.data(), raw.size());

	ImGui::Unindent();
}
