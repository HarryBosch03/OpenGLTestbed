#include "Input.h"

#include "KeyTable.h"
#include "Graphics.h"
#include "Application.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"

#include <map>
#include "InputDevice.h"

MouseDevice mouse;
InputDevice keyboard;

void MouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseButtonEvent(button, action != GLFW_RELEASE);

	if (io.WantCaptureMouse) return;

	mouse.ButtonCallback(button, action != GLFW_RELEASE);
}

void KeyCallback(GLFWwindow* window, int keycode, int scancode, int action, int mods)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiKey imkey = GlfwToImGuiKey(keycode);
	io.AddKeyEvent(imkey, action != GLFW_RELEASE);

	if (io.WantCaptureKeyboard) return;

	keyboard.ButtonCallback(keycode, action != GLFW_RELEASE);
}

void ScrollCallback(GLFWwindow* window, double x, double y)
{
	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseWheelEvent(x, y);

	if (io.WantCaptureMouse) return;

	mouse.ScrollCallback(x, y);
}

void Input::Init()
{
	GLFWwindow* window = Application::Current()->Window();

	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetScrollCallback(window, ScrollCallback);
}

void Input::Update()
{
	mouse.Update();
}

void Input::DrawGUI()
{
	if (!ImGui::CollapsingHeader("Input")) return;

	ImGui::Indent();

	if (ImGui::CollapsingHeader("Mouse"))
	{
		ImGui::Indent();

		ImGui::Text(("Position: (" + std::to_string(Input::Mouse().Position().x) + ", " + std::to_string(Input::Mouse().Position().y) + ")").c_str());
		ImGui::Text(("Scroll: (" + std::to_string(Input::Mouse().Scroll().x) + ", " + std::to_string(Input::Mouse().Scroll().y) + ")").c_str());

		ImGui::Unindent();
	}

	ImGui::Unindent();
}

const MouseDevice& Input::Mouse() { return mouse; }
const InputDevice& Input::Keyboard() { return keyboard; }
