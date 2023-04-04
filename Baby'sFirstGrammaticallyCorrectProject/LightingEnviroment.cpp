#include "LightingEnviroment.h"

#include "Graphics.h"
#include "GLuniform.h"
#include "UniformBufferObject.h"

LightingEnviroment* LightingEnviroment::Current = nullptr;

struct DLightData
{
	Vec4I DLightCount = Zero;
	Vec4 DLightDirections[MaxDLights] = { Zero };
	Vec4 DLightColors[MaxDLights] = { Zero };
	Vec4 AmbientLight = Zero;
};

Vec3 LightingEnviroment::Ambient()
{
	return Uniform::Get<Vec3>("AmbientLight");
}

void LightingEnviroment::Initalize()
{

}

void LightingEnviroment::PushLight(Vec3 direction, Vec3 color)
{
	DLightData& lightData = UniformBufferObject::Lookup<DLightData>("DLightData");
	int& count = lightData.DLightCount[0];

	if (count >= MaxDLights) return;
	
	lightData.DLightDirections[count] = Vector(direction);
	lightData.DLightColors[count] = Vector(color);

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
	Uniform::Set<int>("DLightCount", 0);
}

void LightingEnviroment::SetShaderUniforms(ShaderProgram& shader)
{

}
