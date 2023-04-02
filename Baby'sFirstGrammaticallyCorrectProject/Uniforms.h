#pragma once

#include "GLuniform.h"
#include "Graphics.h"
#include "LightingEnviroment.h"

namespace Uniforms
{
	extern GLuniform<int>
		DLightCount;

	extern GLuniform<float>
		Time;

	extern GLuniform<Vec3>
		CamPos,
		AmbientLight;

	extern GLuniform<Mat4>
		ModelMat,
		ViewMat,
		ProjMat,
		ViewProjMat,
		ModelViewProjMat;

	extern GLuniformList<Vec3>
		DLightDirections,
		DLightColors;
}