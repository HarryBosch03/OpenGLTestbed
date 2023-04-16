#include "Window.h"

#include "Graphics.h"
#include "LogMaster.h"

Window* current = nullptr;

const int startingResolution[2] = { 1280, 720 };
const char* windowTitle = "Game";

void WindowResizeCallback(GLFWwindow* window, int width, int height);

void Window::Initalize()
{
	if (!glfwInit())
	{
		LogError("Failed to initalize GLFW");
		return;
	}

	window = glfwCreateWindow(startingResolution[0], startingResolution[1], windowTitle, nullptr, nullptr);
	if (!window)
	{
		LogError("Failed to CreateWindow");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGL())
	{
		LogError("Failed to load GLAD");
		return;
	}

	glfwSetFramebufferSizeCallback(window, WindowResizeCallback);

	babysitter.Initalize();
	LogSuccess("Successfully Initalized Window");
}

void Window::FinishRender()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Window::Cleanup()
{
	glfwTerminate();
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(window);
}

const Window& Window::Current()
{
	return *current;
}

void WindowResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}