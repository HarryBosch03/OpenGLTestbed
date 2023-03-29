#pragma once

#include "Graphics.h"
#include "GLBabysitter.h"
#include "CameraController.h"
#include "LightingEnviroment.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "MeshRenderData.h"
#include "MeshInstance.h"
#include "RenderTarget.h"

class Application
{
	GLFWwindow* window = nullptr;
	Camera camera;
	CameraController cameraController;
	GLBabysitter babysitter;
	LightingEnviroment lightingEnviroment;
	ShaderProgram shader;
	RenderTarget rt;

	MeshRenderData canRenderData;
	MeshInstance can;

	float time = 0.0f;
	float unscaledTime = 0.0f;
	float frameTime = 0.0f;
	float unscaledFrameTime = 0.0f;
	float fixedUnscaledFrameTime = 1.0f / 50.0f;
	
	float timeScale = 1.0f;
	float fixedFrameTimeAccumilator = 0.0f;

	bool quit = false;

	void Initalize();
	void Loop();
	void FixedLoop();
	void Render();
	bool ShouldClose();

	void HotReloadShaders();

public:
	inline float Time() const { return time; }
	inline float FrameTime() const { return frameTime; }
	inline float& FixedFrameTime() { return fixedUnscaledFrameTime; }

	inline GLFWwindow* Window() const { return window; }

	Application() = default;
	Application(const Application& other) = delete;
	Application& operator =(const Application& other) = delete;
	~Application();

	void Run();
	inline void Quit() { quit = true; }

	static Application* Current;
};