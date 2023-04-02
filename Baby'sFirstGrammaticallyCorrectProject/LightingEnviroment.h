#pragma once

#include "GLuniform.h"
#include "ColorUtil.h"

#include <vector>

class LightingEnviroment
{
private:
	const int maxDLights = 4;

	GLuniform<int> dLightCount = {"dLightCount"};
	GLuniformList<Vec3>
		dLightDirections = {"dLightDirections", maxDLights},
		dLightColors = {"dLightColors", maxDLights};
	GLuniform<Vec3> ambientLight = {"ambientLight"};

public:
	inline Vec3 Ambient() { return ambientLight.value; }

	void Initalize();

	void PushLight(Vec3 direction, Vec3 color);
	void SetAmbient(Vec3 color);
	inline void SetAmbient(Vec3 color, float strength) { SetAmbient(color * strength); }

	void Bind();
	void Unbind();

	void SetShaderUniforms(ShaderProgram& shader);

	static LightingEnviroment* Current;
};