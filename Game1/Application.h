#pragma once

#include "Window.h"
#include "AssetDatabase.h"

class Application
{
	Window window;
	AssetDatabase assets;

	float timeScale = 1.0f;
	float frameTime = 0.0f;
	float fixedFrameTime = 0.02f;

	float frameAccumilator = 0.0f;

	void Initalize();
	void Update();
	void FixedUpdate();
	void Render();
	void Cleanup();

	bool ShouldClose();

public:
	void Run();

	inline const float& TimeScale() const { return timeScale; }
	inline float& TimeScale() { return timeScale; }

	inline const float FrameTime() const { return frameTime; }

	inline const float& FixedFrameTime() const { return fixedFrameTime; }
	inline float& FixedFrameTime() { return fixedFrameTime; }

	inline const AssetDatabase& Assets() const { return assets; }

	static const Application& Current();
};

