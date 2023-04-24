#include "Application.h"

#include "LogMaster.h"
#include "Input.h"
#include "ShaderPreprocessor.h"
#include "AssetDatabasePredicate.h"
#include "RenderProfillingContext.h"
#include "ColorUtility.h"
#include "Scene.h"
#include "MeshInstance.h"
#include "Mesh.h"
#include "DirectionalLight.h"
#include "PointLight.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <chrono>

Application* current = nullptr;

void WindowResizeCallback(GLFWwindow* window, int width, int height);

MeshInstance* soulspear;
Vec3 ambientColor = One;
float ambientStrength = 1.0f;

void Application::Run()
{
	Initalize();

	while (!ShouldClose())
	{
		Logger.PushToConsole();

		float newTime = (float)glfwGetTime();

		unscaledFrameTime = newTime - time;
		frameTime = unscaledFrameTime * timeScale;

		unscaledTime += unscaledFrameTime;
		time += frameTime;
		frame++;

		fixedFrameTimeAccumilator += unscaledFrameTime;

		current = this;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		Loop();

		while (fixedFrameTimeAccumilator >= fixedUnscaledFrameTime)
		{
			FixedLoop();
			fixedFrameTimeAccumilator -= fixedUnscaledFrameTime;
		}

		ImGui::Render();

		Render();
	}
}

void Close(void* data)
{

}

void Application::Initalize()
{
	current = this;

	if (!glfwInit()) return;

	GLFWmonitor* monitor = nullptr;
	int width = 1280, height = 720;

#if _DEBUG
	int monitorCount;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
	monitor = monitorCount > 1 ? monitors[1] : nullptr;
	glfwGetMonitorWorkarea(monitor, nullptr, nullptr, &width, &height);
#endif

	window = glfwCreateWindow(width, height, "Trust me I Didnt", monitor, nullptr);

	if (!window)
	{
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGL()) return;

	glfwSetFramebufferSizeCallback(window, WindowResizeCallback);

	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	babysitter.Initalize();

	ShaderPreprocessor::Initalize();

	soulspear = new MeshInstance();

	Mesh* monkeMesh = GetAsset<Mesh>("Models/soulspear.obj", nullptr);
	soulspear->SetMeshData(monkeMesh);

	Material material = Material("shader");
	material.SetTexture("texCol", GetAsset<Texture>("Textures/SoulSpear/soulspear_diffuse.tga"));
	material.SetTexture("texNormal", GetAsset<Texture>("Textures/SoulSpear/soulspear_normal.tga"));
	soulspear->SetMaterials(material);

	renderPipeline.skybox.Setup();
	renderPipeline.lighting.Initalize();
	renderPipeline.lighting.enviromentTex = GetAsset<Texture>("Textures/forest.hdr");

	new DirectionalLight({ 1.0, -1.0, -1.0 }, Utility::Color::Hex3(0xffefd6), 1.0f);
	new DirectionalLight({ -1.0, 1.0, 0.5 }, Utility::Color::Hex3(0xc4eeff), 0.6f);
	new DirectionalLight({ -1.0, -0.2, 0.5 }, Utility::Color::Hex3(0xc4eeff), 0.3f);

	new PointLight({}, Vec3(255.0, 15.0, 0.0) / 255.0f, 100.0f);

	Input::Init();
}

float metalIn = 0.0f, roughIn = 0.0f;

void Application::Loop()
{
	Input::Update();

	ImGui::Begin("Inspector");

	cameraController.Control(camera);
	renderPipeline.lighting.SetAmbient(ambientColor, ambientStrength);

	if (ImGui::CollapsingHeader("Scene Objects"))
	{
		ImGui::Indent();

		for (SceneObject* object : SceneObject::All())
		{
			object->DrawGUI();
		}
		ImGui::Unindent();
	}

	for (SceneObject* object : SceneObject::All())
	{
		if (!object->enabled) continue;
		object->Update();
	}

	if (ImGui::CollapsingHeader("Utility"))
	{
		ImGui::Indent();

		if (ImGui::CollapsingHeader("GI"))
		{
			ImGui::ColorEdit3("Ambient Color", &ambientColor.x);
			ImGui::DragFloat("Ambient Strength", &ambientStrength, 0.1f, 0.0f, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp);
		}

		Uniform::Set<float>("metalIn", metalIn);
		Uniform::Set<float>("roughIn", roughIn);

		if (ImGui::Button("Hot Reload All Assets"))
		{
			Assets().HotReload();
		}

		for (int i = 1; i < (int)AssetType::Count; i++)
		{
			if (ImGui::Button(("Reload " + AssetTypenames[i] + "s").c_str()))
			{
				Assets().HotReload(AssetDatabasePredicates::MatchType((AssetType)i));
			}
		}

		ImGui::Checkbox("Persist Console", &persistConsole);

		Input::DrawGUI();
		cameraController.DrawGUI();

		ImGui::Unindent();
	}

	RenderProfillingContext::RenderGUI();
	ImGui::End();

	if (Input::Keyboard().Down(GLFW_KEY_ESCAPE)) quit = true;
}

void Application::FixedLoop()
{

}

void Application::Render()
{
	renderPipeline.Render();
}

bool Application::ShouldClose()
{
	return quit || glfwWindowShouldClose(window);
}

void WindowResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Application::~Application()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}

const Application* Application::Current()
{
	return current;
}