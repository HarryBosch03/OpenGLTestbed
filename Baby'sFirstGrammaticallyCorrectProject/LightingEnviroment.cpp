#include "LightingEnviroment.h"

#include "Graphics.h"

LightingEnviroment* LightingEnviroment::Current = nullptr;

Vec3 LightingEnviroment::Ambient()
{
	return Uniforms::AmbientLight.value;
}

void LightingEnviroment::Initalize()
{

}

void LightingEnviroment::PushLight(Vec3 direction, Vec3 color)
{
	if (Uniforms::DLightCount.value >= MaxDLights) return;

	Uniforms::DLightDirections[Uniforms::DLightCount] = direction;
	Uniforms::DLightColors[Uniforms::DLightCount] = color;
	Uniforms::DLightCount.value++;
}

void LightingEnviroment::SetAmbient(Vec3 color)
{
	Uniforms::AmbientLight.value = color;
}

void LightingEnviroment::Bind()
{
	Current = this;
}

void LightingEnviroment::Unbind()
{
	Current = nullptr;
	Uniforms::DLightCount = 0;
}

void LightingEnviroment::SetShaderUniforms(ShaderProgram& shader)
{

}
