#pragma once

#include "ColorUtil.h"

#include <vector>

const int MaxDLights = 4;

class ShaderProgram;

class LightingEnviroment
{
public:
	Vec3 Ambient();

	void Initalize();

	void PushLight(Vec3 direction, Vec3 color);
	void SetAmbient(Vec3 color);
	inline void SetAmbient(Vec3 color, float strength) { SetAmbient(color * strength); }

	void Bind();
	void Unbind();

	void SetShaderUniforms(ShaderProgram& shader);

	static LightingEnviroment* Current;
};