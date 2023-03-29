#include "Input.h"

#include "KeyTable.h"
#include "Graphics.h"
#include "Application.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"

#include <map>

#define MAP_FUNCTIONS_DEF(name) \
std::map<int, bool> name ## Map; \
std::map<int, bool> last ## name ## Map; \
bool Input::Get ## name ## Down(int key) \
{ \
	return name ## Map[key]; \
} \
bool Input::Get ## name ## Pressed(int key) \
{ \
	return name ## Map[key] && !last ## name ## Map[key]; \
} \
bool Input::Get ## name ## Released(int key) \
{ \
	return !name ## Map[key] && !last ## name ## Map[key]; \
}

#define MAP_UPDATE(name) \
for (auto it = name ## Map.begin(); it != name ## Map.end(); ++it) \
{ \
	last ## name ## Map[it->first] = it->second; \
}

MAP_FUNCTIONS_DEF(Key)
MAP_FUNCTIONS_DEF(Mouse)

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
	glfwSetMouseButtonCallback(Application::Current->Window(), MouseCallback);
	glfwSetKeyCallback(Application::Current->Window(), KeyCallback);
}

void Input::Update()
{
	MAP_UPDATE(Key);
	MAP_UPDATE(Mouse);
}