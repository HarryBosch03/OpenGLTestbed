#include "RenderPipeline.h"

#include "RenderProfillingContext.h"
#include "Application.h"
#include "Graphics.h"
#include "Scene.h"

#include <chrono>
#include <imgui.h>
#include <imgui_impl_opengl3.h>

GLFWwindow* window = nullptr;
int width = 0, height = 0;

void ClearRT()
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void RenderGUI()
{
	auto data = ImGui::GetDrawData();
	if (data)
	{
		ImGui_ImplOpenGL3_RenderDrawData(data);
	}
}

void RenderCamera(RenderPipeline& pipeline, Camera* camera)
{
	camera->Bind();
	pipeline.skybox.Draw();
	for (Scene* scene : Scene::Scenes())
	{
		scene->Render();
	}
	camera->Unbind();
}

void RenderPipeline::Initalize()
{

}

void RenderPipeline::Render()
{
	RenderProfillingContext::Start("Render Loop");

	window = Application::Current().Window();
	glfwGetWindowSize(window, &width, &height);

	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	lighting.Bind();
	skybox.Bind();

	for (Camera* camera : Camera::Itterator())
	{
		RenderCamera(*this, camera);
	}

	lighting.Unbind();
	skybox.Unbind();

	RenderGUI();

	glfwSwapBuffers(window);
	glfwPollEvents();

	RenderProfillingContext::Finalize();
}
