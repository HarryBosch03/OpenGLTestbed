#pragma once

#include "GLBabysitter.h"
#include "CameraController.h"
#include "Camera.h"
#include "RenderPipeline.h"

class Application
{
	GLFWwindow* window = nullptr;
	Camera camera;
	CameraController cameraController;
	GLBabysitter babysitter;
	RenderPipeline renderPipeline;

	float time = 0.0f;
	float unscaledTime = 0.0f;
	float frameTime = 0.0f;
	float unscaledFrameTime = 0.0f;
	float fixedUnscaledFrameTime = 1.0f / 50.0f;
	int frame = -1;

	float timeScale = 1.0f;
	float fixedFrameTimeAccumilator = 0.0f;

	bool quit = false;

	void Initalize();
	void Loop();
	void FixedLoop();
	void Render();
	bool ShouldClose();

public:
	bool persistConsole = false;

	inline static const float Time() { return Application::Current()->time; }
	inline static const float FrameTime() { return Application::Current()->frameTime; }
	inline static const float FixedFrameTime() { return Application::Current()->fixedUnscaledFrameTime; }
	inline static const int Frame() { return Application::Current()->frame; }

	inline GLFWwindow* Window() const { return window; }

	Application() = default;
	Application(const Application& other) = delete;
	Application& operator =(const Application& other) = delete;
	~Application();

	void Run();
	inline void Quit() { quit = true; }

	static const Application* Current();
};