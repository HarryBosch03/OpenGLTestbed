#include "Application.h"

#include "MeshData.h"
#include "MeshRenderData.h"

#include "LogMaster.h"
#include "Input.h"
#include "ShaderPreprocessor.h"
#include "DrawGUIListener.h"
#include "AssetDatabasePredicate.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

Application* Application::Current = nullptr;

void WindowResizeCallback(GLFWwindow* window, int width, int height);

Vec3 ambient = GREY(1.0);
float ambientStrength = 1.0f;
const int dLightCount = 3;
Vec3 dLightColor[dLightCount] =
{
	HEX3(ffefd6),
	HEX3(c4eeff),
	HEX3(c4eeff),
};
float dLightStrength[dLightCount] =
{
	1.0f,
	0.6f,
	0.3f,
};

Vec3 pointLightPosition = {};
Vec3 pointLightColor = Vec3(255.0, 15.0, 0.0) / 255.0f;
float pointLightValue = 100.0f;

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

		Current = this;

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
	Application::Current = this;

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

	LogMessage("Initalized Window Context");

	ShaderPreprocessor::Initalize();
	LogMessage("Initalized Shader Preprocessor");

	renderData.Load(AssetDatabase::LoadAsset<MeshData>("Models/Monke Low Res.fbx", nullptr));
	LogMessage("Loaded Monke Mesh");

	Material material = Material("shader");
	can.SetMaterial(material).SetMeshData(&renderData);
	LogMessage("Loaded Monke Material");

	can.material.SetTexture("texCol", AssetDatabase::LoadAsset<Texture>("Textures/Monke Metal 2/Monke.Albedo.tga"));
	can.material.SetTexture("texMetal", AssetDatabase::LoadAsset<Texture>("Textures/Monke Metal 2/Monke.Metal.tga"));
	can.material.SetTexture("texRough", AssetDatabase::LoadAsset<Texture>("Textures/Monke Metal 2/Monke.Rough.tga"));
	can.material.SetTexture("texNormal", AssetDatabase::LoadAsset<Texture>("Textures/Monke Metal 2/Monke.Normal.tga"));
	can.material.SetTexture("texHeight", AssetDatabase::LoadAsset<Texture>("Textures/Monke Metal 2/Monke.Height.tga"));
	can.material.SetTexture("texAO", AssetDatabase::LoadAsset<Texture>("Textures/Monke Metal 2/Monke.AO.tga"));
	LogMessage("Loaded Monke Textures");

	skybox.Setup("Textures/forest.hdr");
	LogMessage("Loaded Skybox");

	lightingEnviroment.Initalize();
	LogMessage("Initlalized Lighting Enviroment");

	Input::Init();
	LogMessage("Initlalized Input");
}

float metalIn = 0.0f, roughIn = 0.0f;

void Application::Loop()
{
	ImGui::Begin("Utility");

	can.position = Vec3(0.0f, glm::sin(Time() * 5.0f) * 0.2f, 0.0f);

	float a = Time() * 1.0f;
	can.axisAngleRotation = Vec4(1.0f, 0.0f, 0.0f, -90.0f);
	can.scale = One * 0.6f;

	cameraController.Control(camera);

	lightingEnviroment.PushDirectionalLight({ 1.0, -1.0, -1.0 }, dLightColor[0] * dLightStrength[0]);
	lightingEnviroment.PushDirectionalLight({ -1.0, 1.0, 0.5 }, dLightColor[1] * dLightStrength[1]);
	lightingEnviroment.PushDirectionalLight({ -1.0, -0.2, 0.5 }, dLightColor[2] * dLightStrength[2]);

	lightingEnviroment.PushPointLight(pointLightPosition, pointLightColor * pointLightValue);

	lightingEnviroment.SetAmbient(ambient, ambientStrength);

	if (ImGui::CollapsingHeader("Lighting Enviroment"))
	{
		ImGui::Indent();

		ImGui::ColorEdit3("Ambient Light", &ambient.x);
		ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0.0f, 2.0f);
		for (size_t i = 0; i < dLightCount; i++)
		{
			if (!ImGui::CollapsingHeader((std::string("Light.") + std::to_string(i)).c_str())) continue;

			ImGui::Indent();

			ImGui::ColorEdit3("Color##" + i, &dLightColor[i].x);
			ImGui::SliderFloat("Strength##" + i, &dLightStrength[i], 0.0f, 10.0f);

			ImGui::Unindent();
		}

		if (ImGui::CollapsingHeader("Point Light"))
		{
			ImGui::Indent();

			ImGui::InputFloat3("Position", &pointLightPosition.x);
			ImGui::ColorEdit3("Color##Point Light", &pointLightColor.x);
			ImGui::InputFloat("Strength##Point Light", &pointLightValue);

			ImGui::Unindent();
		}

		ImGui::Unindent();
	}

	Uniform::Set<float>("metalIn", metalIn);
	Uniform::Set<float>("roughIn", roughIn);

	DrawGUIListener::DrawAllListeners();

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

	Input::Update();
	ImGui::End();
}

void Application::FixedLoop()
{

}

void Application::Render()
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	glClearColor(ambient.r, ambient.g, ambient.b, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	camera.Bind();

	lightingEnviroment.Bind();
	skybox.Bind();

	can.position = Right;
	can.Draw();
	can.position = Up;
	can.Draw();
	can.position = Forward;
	can.Draw();

	skybox.Draw();

	skybox.Unbind();
	lightingEnviroment.Unbind();

	camera.Unbind();

	auto data = ImGui::GetDrawData();
	if (data)
	{
		ImGui_ImplOpenGL3_RenderDrawData(data);
	}

	glfwSwapBuffers(window);
	glfwPollEvents();
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