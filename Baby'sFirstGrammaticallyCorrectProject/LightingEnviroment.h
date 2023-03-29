#pragma once

#include "GLuniform.h"
#include "ColorUtil.h"

#include <vector>

class LightingEnviroment
{
private:
	const int maxDLights = 4;

	GL_UNIFORM_SYNC_GROUP_DEF

	SYNC_GROUP_ARGS(GLuInt, dLightCount, "dLightCount", 0)
	SYNC_GROUP_ARGS(GLuVec3L, dLightDirections, "dLightDirections", Zero COMMA maxDLights)
	SYNC_GROUP_ARGS(GLuVec3L, dLightColors, "dLightColors", Zero COMMA maxDLights)
	SYNC_GROUP_ARGS(GLuVec3, ambientLight, "ambientLight", Zero)

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