#include "Input.h"

#include "KeyTable.h"
#include "Graphics.h"
#include "Application.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"

#include <map>

void MouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseButtonEvent(button, action != GLFW_RELEASE);

	if (io.WantCaptureMouse) return;

	MouseMap[button] = action != GLFW_RELEASE;
}

void KeyCallback(GLFWwindow* window, int keycode, int scancode, int action, int mods)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiKey imkey = GlfwToImGuiKey(keycode);
	io.AddKeyEvent(imkey, action != GLFW_RELEASE);

	if (io.WantCaptureKeyboard) return;

	KeyMap[keycode] = action != GLFW_RELEASE;
}

void Input::Init()
{
	GLFWwindow* window = Application::Current()->Window();

	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwSetKeyCallback(window, KeyCallback);
}

void Input::Update()
{
	mouse.Update();
}

bool Input::InputDevice::GetDown(int key)
{
	return cMap[key];
}

bool Input::InputDevice::GetPressed(int key)
{
	return cMap[key] && !lMap[key];
}

bool Input::InputDevice::GetReleased(int key)
{
	return !cMap[key] && lMap[key];
}

void Input::InputDevice::Update()
{
	lMap = cMap;
}

void Input::Mouse::Initalize()
{
	glfwSetScrollCallback(Window(), );
}

void Input::Mouse::Update()
{
	InputDevice::Update();

	lastPosition = position;
	lastScroll = scroll;

	glm::vec<2, double, glm::defaultp> dpos;
	glfwGetCursorPos(Window(), &dpos.x, &dpos.y);
	position = dpos;

	
}