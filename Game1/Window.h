#pragma once

#include "GLBabysitter.h"

class GLFWwindow;

class Window
{
	GLFWwindow* window;
	GLBabysitter babysitter;

public:
	void Initalize();
	void FinishRender();
	void Cleanup();

	bool ShouldClose();

	static const Window& Current();
};

