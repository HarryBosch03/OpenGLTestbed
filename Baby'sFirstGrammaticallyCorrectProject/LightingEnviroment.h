#pragma once

#include "Maths.h"

#include <vector>

const int MaxDLights = 4;
const int MaxLights = 64;

class ShaderProgram;

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

class LightingEnviroment
{
public:
	DLightData dLightData;
	LightData lightData;

	void Initalize();

	void SetAmbient(Vec3 color);
	inline void SetAmbient(Vec3 color, float strength) { SetAmbient(color * strength); }

	void Bind();
	void Unbind();

	static LightingEnviroment* Current;
};