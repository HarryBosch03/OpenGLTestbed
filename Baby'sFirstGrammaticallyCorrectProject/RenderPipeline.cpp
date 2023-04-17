#include "RenderPipeline.h"

#include "RenderProfillingContext.h"
#include "Application.h"
#include "Graphics.h"
#include "MeshInstance.h"

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
	for (SceneObject* object : SceneObject::All())
	{
		object->Draw();
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

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

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
