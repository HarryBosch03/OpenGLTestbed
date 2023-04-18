#include "SceneObject.h"

#include <iostream>

#include "imgui.h"
#include "InspectorUtility.h"

using namespace Utility::Inspector;

std::vector<SceneObject*> instances;
std::vector<SceneObject*> enabledList;
int nextID = 0;

void SceneObject::SetTypename(const std::string & _typename)
{
	name = _typename + "." + std::to_string(id);
}

SceneObject::SceneObject() : id(nextID++)
{
	scene = nullptr;
	instances.push_back(this);
	SetTypename("Scene Object");
}

SceneObject::~SceneObject()
{
	for (int i = 0; i < instances.size(); i++)
	{
		const SceneObject* other = instances[i];
		if (other != this) continue;
		instances.erase(instances.begin() + i);
		break;
	}
}

const std::vector<SceneObject*>& SceneObject::All(bool includeDisabled)
{
	if (includeDisabled)
	{
		for (SceneObject* object : instances)
		{
			if (!object->enabled) continue;
			enabledList.push_back(object);
		}

		return enabledList;
	}
	return instances;
}

void SceneObject::DrawGUI()
{
	WorkingID() = this->id;
	if (!ImGui::CollapsingHeader(AppendID(name))) return;
	ImGui::Indent();
	DrawGUIContent();
	ImGui::Unindent();
}

void SceneObject::DrawGUIContent()
{
	ImGui::Checkbox(AppendID("Enabled"), &enabled);
}