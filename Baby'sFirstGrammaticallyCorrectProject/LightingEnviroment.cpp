#include "LightingEnviroment.h"

#include "Graphics.h"
#include "GLuniform.h"

LightingEnviroment* LightingEnviroment::Current = nullptr;

Vec3 LightingEnviroment::Ambient()
{
	return Uniform::Get<Vec3>("AmbientLight");
}

void LightingEnviroment::Initalize()
{

}

void LightingEnviroment::PushLight(Vec3 direction, Vec3 color)
{
	int& count = Uniform::Get<int>("DLightCount");
	if (count >= MaxDLights) return;

	Uniform::SetBuffer<Vec3>("DLightDirections", MaxDLights, count, direction);
	Uniform::SetBuffer<Vec3>("DLightColors", MaxDLights, count, color);
	count++;
}

void LightingEnviroment::SetAmbient(Vec3 color)
{
	Uniform::Set<Vec3>("AmbientLight", color);
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
