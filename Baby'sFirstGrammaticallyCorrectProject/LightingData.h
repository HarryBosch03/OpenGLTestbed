#pragma once

#include "Maths.h"
#include "DirectionalLightData.h"

struct LightingData
{
	static const int MaxDirectionalLights = 4;

	Vec3 ambientLight;
	int directionalLightCount;
	DirectionalLightData directionalLightData[MaxDirectionalLights];
};