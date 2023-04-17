#include "Application.h"

#include "LogMaster.h"
#include "Input.h"
#include "ShaderPreprocessor.h"
#include "DrawGUIListener.h"
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

MeshInstance* monke;

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

	window = glfwCreateWindow(1280, 720, "Trust me I Didnt", nullptr, nullptr);
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

	monke = new MeshInstance();

	Mesh* monkeMesh = AssetDatabase::LoadAsset<Mesh>("Models/Monke Low Res.fbx", nullptr);
	monke->SetMeshData(monkeMesh);

	Material material = Material("shader");
	material.SetTexture("texCol", AssetDatabase::LoadAsset<Texture>("Textures/Monke Metal 2/Monke.Albedo.tga"));
	material.SetTexture("texMetal", AssetDatabase::LoadAsset<Texture>("Textures/Monke Metal 2/Monke.Metal.tga"));
	material.SetTexture("texRough", AssetDatabase::LoadAsset<Texture>("Textures/Monke Metal 2/Monke.Rough.tga"));
	material.SetTexture("texNormal", AssetDatabase::LoadAsset<Texture>("Textures/Monke Metal 2/Monke.Normal.tga"));
	material.SetTexture("texHeight", AssetDatabase::LoadAsset<Texture>("Textures/Monke Metal 2/Monke.Height.tga"));
	material.SetTexture("texAO", AssetDatabase::LoadAsset<Texture>("Textures/Monke Metal 2/Monke.AO.tga"));
	monke->SetMaterials(material);

	Scene& scene = *new Scene("Main Scene");
	scene.Add(monke);

	renderPipeline.skybox.Setup("Textures/forest.hdr");
	renderPipeline.lighting.Initalize();

	new DirectionalLight({ 1.0, -1.0, -1.0 }, Utility::Color::Hex3(0xffefd6), 1.0f);
	new DirectionalLight({ -1.0, 1.0, 0.5 }, Utility::Color::Hex3(0xc4eeff), 0.6f);
	new DirectionalLight({ -1.0, -0.2, 0.5 }, Utility::Color::Hex3(0xc4eeff), 0.3f);

	new PointLight({}, Vec3(255.0, 15.0, 0.0) / 255.0f, 100.0f);

	Input::Init();
}

float metalIn = 0.0f, roughIn = 0.0f;

void Application::Loop()
{
	ImGui::Begin("Inspector");

	monke->position = Vec3(2.0f, glm::sin(Time() * 5.0f) * 0.2f, 0.0f);

	float a = Time() * 1.0f;
	monke->axisAngleRotation = Vec4(1.0f, 0.0f, 0.0f, -90.0f);
	monke->scale = One * 0.6f;

	cameraController.Control(camera);

	renderPipeline.lighting.SetAmbient(One, 1.0f);

	for (Scene* scene : Scene::Scenes())
	{
		scene->Update();
	}

	if (ImGui::CollapsingHeader("Utility"))
	{
		ImGui::Indent();
		
		Uniform::Set<float>("metalIn", metalIn);
		Uniform::Set<float>("roughIn", roughIn);

		if (ImGui::Button("Hot Reload All Assets"))
		{
			AssetDatabase::HotReload();
		}

		for (int i = 1; i < (int)AssetType::Count; i++)
		{
			if (ImGui::Button(("Reload " + AssetTypenames[i] + "s").c_str()))
			{
				AssetDatabase::HotReload(AssetDatabase::Predicates::MatchType((AssetType)i));
			}
		}

		ImGui::Unindent();
	}

	DrawGUIListener::DrawAllListeners();
	RenderProfillingContext::RenderGUI();
	ImGui::End();

	Input::Update();
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

const Application& Application::Current()
{
	return *current;
}