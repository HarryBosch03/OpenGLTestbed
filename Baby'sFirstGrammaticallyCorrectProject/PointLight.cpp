#include "PointLight.h"

#include "InspectorUtility.h"

#include "imgui.h"

using namespace Utility::Inspector;

PointLight::PointLight(Vec3 position, Vec3 color, float brightness) :
	position(position),
	color(color),
	brightness(brightness) 
{
	SetTypename("Point Light");
}

void PointLight::DrawGUIContent()
{
	SceneObject::DrawGUIContent();
	ImGui::ColorEdit3(AppendID("Color"), &color.x);
	ImGui::DragFloat(AppendID("Brightness"), &brightness);
}
