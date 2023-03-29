#include "LightingEnviroment.h"

#include "Graphics.h"

LightingEnviroment* LightingEnviroment::Current = nullptr;

void LightingEnviroment::Initalize()
{

}

void LightingEnviroment::PushLight(Vec3 direction, Vec3 color)
{
	if (dLightCount.value >= maxDLights) return;

	dLightDirections[dLightCount] = direction; 
	dLightColors[dLightCount] = color;
	dLightCount.value++;
}

void LightingEnviroment::SetAmbient(Vec3 color)
{
	ambientLight.value = color;
}

void LightingEnviroment::Bind()
{
	Current = this;
	GL_UNIFORM_SYNC_ALL
}

void LightingEnviroment::Unbind()
{
	Current = nullptr;
	dLightCount = 0;
}

void LightingEnviroment::SetShaderUniforms(ShaderProgram& shader)
{

}
