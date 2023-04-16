#include "Application.h"

#include "LogMaster.h"

Application* current = nullptr;

void Application::Initalize()
{
	window.Initalize();
	assets.PreloadAll();
}

void Application::Update()
{

}

void Application::FixedUpdate()
{

}

void Application::Render()
{
	window.FinishRender();
}

void Application::Cleanup()
{
	window.Cleanup();
}

bool Application::ShouldClose()
{
	return window.ShouldClose();
}

void Application::Run()
{
	current = this;

	Initalize();

	while (!ShouldClose())
	{
		while (frameAccumilator > fixedFrameTime)
		{
			FixedUpdate();
			frameAccumilator -= fixedFrameTime;
		}

		Update();

		Render();

		Logger.PushToConsole();
	}

	Cleanup();
}

const Application& Application::Current()
{
	return *current;
}
