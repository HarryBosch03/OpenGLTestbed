#pragma once

#include "DirectionalLight.h"
#include "GLuniform.h"
#include "ColorUtil.h"
#include "LightingData.h"
#include "ShaderBuffer.h"

#include <vector>

class LightingEnviroment
{
private:
	ShaderBuffer dataBuffer;
	LightingData data;
	int directionalLightCount;

public:
	
	Vec3 ambientColor = ColorUtil::FromHex3(0xc4eeff);
	float ambientStrength = 0.2f;
	
	inline Vec3 Ambient() { return ambientColor * ambientStrength; }

	void Initalize();

	void PushLight(const DirectionalLightData& light);
	void Bind();
	void Unbind();

	void SetShaderUniforms(ShaderProgram& shader);

	static LightingEnviroment* Current;
};