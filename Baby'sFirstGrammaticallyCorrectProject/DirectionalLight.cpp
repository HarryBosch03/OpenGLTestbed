#include "DirectionalLight.h"

#include "Application.h"
#include "InspectorUtility.h"

#include "imgui/imgui.h"

using namespace Utility::Inspector;

DirectionalLight::DirectionalLight(Vec3 direction, Vec3 color, float brightness) : 
	direction(direction), 
	color(color), 
	brightness(brightness) 
{
	SetTypename("Directional Light");
}

void DirectionalLight::DrawGUIContent()
{
	SceneObject::DrawGUIContent();
	ImGui::ColorEdit3(AppendID("Color"), &color.x);
	ImGui::DragFloat(AppendID("Brightness"), &brightness);
}
