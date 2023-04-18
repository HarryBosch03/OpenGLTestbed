#include "LightingEnviroment.h"

#include "Graphics.h"
#include "GLuniform.h"
#include "UniformBufferObject.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Texture.h"

LightingEnviroment* current = nullptr;

void LightingEnviroment::Initalize()
{

}

void LightingEnviroment::SetAmbient(Vec3 color)
{
	dLightData.AmbientLight = Vec4(color.x, color.y, color.z, 0.0);
}

void GetDirectionalLights(DLightData& lightData)
{
	const std::vector<DirectionalLight*> lights = SceneObject::All<DirectionalLight>();
	for (DirectionalLight* light : lights)
	{
		int& count = lightData.DLightCount[0];

		if (count >= MaxDLights) return;

		lightData.DLightDirections[count] = Vector(light->direction);
		lightData.DLightColors[count] = Vector(light->FinalColor());

		count++;
	}
}

void GetPointLights(LightData& lightData)
{
	const std::vector<PointLight*> lights = SceneObject::All<PointLight>();
	for (PointLight* light : lights)
	{
		int& count = lightData.LightCount[0];

		if (count >= MaxLights) return;

		lightData.LightPositions[count] = Point(light->position);
		lightData.LightColors[count] = Vector(light->FinalColor());
		lightData.LightDirections[count] = Vec4(1.0f, 0.0f, 0.0f, 1.0f);

		count++;
	}
}

void LightingEnviroment::Bind()
{
	current = this;

	dLightData.DLightCount = Zero;
	lightData.LightCount = Zero;

	GetDirectionalLights(dLightData);
	GetPointLights(lightData);

	if (enviromentTex) enviromentTex->Bind("glMap");

	UniformBufferObject::Lookup<DLightData>("DLightData") = dLightData;
	UniformBufferObject::Lookup<LightData>("LightData") = lightData;
}

void LightingEnviroment::Unbind()
{
	current = nullptr;
	if (enviromentTex) enviromentTex->Unbind();
}

const LightingEnviroment* LightingEnviroment::Current()
{
	return current;
}