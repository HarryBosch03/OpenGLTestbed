#include "LightingEnviroment.h"

#include "Graphics.h"
#include "GLuniform.h"
#include "UniformBufferObject.h"

LightingEnviroment* LightingEnviroment::Current = nullptr;

struct DLightData
{
	Vec4I DLightCount = Zero;
	Vec4 DLightDirections[MaxDLights]{};
	Vec4 DLightColors[MaxDLights]{};
	Vec4 AmbientLight = Zero;
};

struct LightData
{
	Vec4I LightCount = Zero;
	Vec4 LightPositions[MaxLights]{};
	Vec4 LightColors[MaxLights]{};
	Vec4 LightDirections[MaxLights]{};
};

Vec3 LightingEnviroment::Ambient()
{
	return Uniform::Get<Vec3>("AmbientLight");
}

void LightingEnviroment::Initalize()
{

}

void LightingEnviroment::PushDirectionalLight(Vec3 direction, Vec3 color)
{
	DLightData& lightData = UniformBufferObject::Lookup<DLightData>("DLightData");
	int& count = lightData.DLightCount[0];

	if (count >= MaxDLights) return;
	
	lightData.DLightDirections[count] = Vector(direction);
	lightData.DLightColors[count] = Vector(color);

	count++;
}

void LightingEnviroment::PushPointLight(Vec3 position, Vec3 color)
{
	LightData& lightData = UniformBufferObject::Lookup<LightData>("LightData");
	int& count = lightData.LightCount[0];

	if (count >= MaxLights) return;

	lightData.LightPositions[count] = Point(position);
	lightData.LightColors[count] = Vector(color);
	lightData.LightDirections[count] = Vec4(1.0f, 0.0f, 0.0f, 1.0f);

	count++;
}

void LightingEnviroment::SetAmbient(Vec3 color)
{
	UniformBufferObject::Lookup<DLightData>("DLightData").AmbientLight = Vector(color);
}

void LightingEnviroment::Bind()
{
	Current = this;
}

void LightingEnviroment::Unbind()
{
	Current = nullptr;
	
	UniformBufferObject::Lookup<DLightData>("DLightData").DLightCount = Zero;
	UniformBufferObject::Lookup<LightData>("LightData").LightCount = Zero;
}

void LightingEnviroment::SetShaderUniforms(ShaderProgram& shader)
{

}
