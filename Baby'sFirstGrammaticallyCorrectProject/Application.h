#pragma once

#include "Graphics.h"
#include "GLBabysitter.h"
#include "CameraController.h"
#include "LightingEnviroment.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "MeshRenderData.h"
#include "MeshInstance.h"

class Application
{
	GLFWwindow* window = nullptr;
	Camera camera;
	CameraController cameraController;
	GLBabysitter babysitter;
	LightingEnviroment lightingEnviroment;
	ShaderProgram shader;

	MeshRenderData canRenderData;
	MeshInstance can;

	float time = 0.0f;
	float frameTime = 0.0f;
	float fixedFrameTime = 1.0f / 50.0f;
	float fixedFrameTimeAccumilator = 0.0f;

	void Initalize();
	void Loop();
	void FixedLoop();
	void Render();

public:
	inline float Time() const { return time; }
	inline float FrameTime() const { return frameTime; }
	inline float& FixedFrameTime() { return fixedFrameTime; }

	inline GLFWwindow* Window() const { return window; }

	void Run();

	static Application* Current;
};