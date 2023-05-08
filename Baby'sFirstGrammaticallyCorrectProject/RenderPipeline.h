#pragma once

#include "Skybox.h"
#include "LightingEnviroment.h"
#include "PostProcessingStack.h"

class RenderPipeline
{
	float renderTime;
	PostProcessingStack stack;

public:
	Skybox skybox;
	LightingEnviroment lighting;
	
	void Initalize();
	void Render();

	inline const float& RenderTime() const { return renderTime; }
};