#include "LightingEnviroment.h"

#include "Graphics.h"

LightingEnviroment* LightingEnviroment::Current = nullptr;

void LightingEnviroment::Initalize()
{
	dataBuffer.Initalize(0, (byte*)&data, sizeof(LightingData));
}

void LightingEnviroment::PushLight(const DirectionalLightData& light)
{
	data.directionalLightData[data.directionalLightCount++] = light;
}

void LightingEnviroment::Bind()
{
	Current = this;
	data.ambientLight = Ambient();
}

void LightingEnviroment::Unbind()
{
	Current = nullptr;
	directionalLightCount = 0;
}

void LightingEnviroment::SetShaderUniforms(ShaderProgram& shader)
{

}
