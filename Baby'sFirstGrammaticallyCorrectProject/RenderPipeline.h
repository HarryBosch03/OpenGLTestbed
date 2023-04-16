#pragma once

#include "Skybox.h"
#include "LightingEnviroment.h"

class RenderPipeline
{
	float renderTime;

public:
	Skybox skybox;
	LightingEnviroment lighting;
	
	void Initalize();
	void Render();

	inline const float& RenderTime() const { return renderTime; }
};