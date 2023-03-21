#include "Application.h"

#include "MeshData.h"
#include "MeshRenderData.h"

Application* Application::Current = nullptr;

void WindowResizeCallback(GLFWwindow* window, int width, int height);

void Application::Run()
{
	Initalize();

	while (!glfwWindowShouldClose(window))
	{
		float newTime = (float)glfwGetTime();
		frameTime = newTime - time;
		time = newTime;

		fixedFrameTimeAccumilator += frameTime;

		Current = this;
		Loop();

		while (fixedFrameTimeAccumilator >= fixedFrameTime)
		{
			FixedLoop();
			fixedFrameTimeAccumilator -= fixedFrameTime;
		}

		Render();
	}
}

void Application::Initalize()
{
	if (!glfwInit()) return;

	window = glfwCreateWindow(1280, 720, "Trust me I Didnt", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGL()) return;

	glfwSetFramebufferSizeCallback(window, WindowResizeCallback);

	babysitter.Initalize();
	shader.Initalize("./Assets/Shaders/shader.vert", "./Assets/Shaders/shader.frag");

	canRenderData.Load("./Assets/Models/soda can.obj");
	can.meshData = &canRenderData;
}

void Application::Loop()
{
	can.position = Vec3(0.0f, glm::sin(Time() * 5.0f) * 0.2f, 0.0f);

	float a = Time() * 1.0f;
	can.axisAngleRotation = Vec4(1.0f, 0.0f, 0.0f, -30.0f);
	can.scale = One * 0.6f;

	cameraController.Control(camera);
}

void Application::FixedLoop()
{

}

void Application::Render()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(lightingEnviroment.ambientColor.r, lightingEnviroment.ambientColor.g, lightingEnviroment.ambientColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera.Bind();
	lightingEnviroment.Bind();
	shader.Bind();

	can.Bind();
	can.Draw();
	can.Unbind();

	shader.Unbind();
	lightingEnviroment.Unbind();
	camera.Unbind();

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void WindowResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}